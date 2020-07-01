//
// Created by dd on 19.06.2020.
//

#include "custom_http_client.h"
#include <memory>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include <vector>

class ErrorString {
    std::string str_;
    UniqPtr<BIO> bio_;
    UniqPtr<BIO_METHOD> bio_meth_;
public:
    ErrorString(ErrorString &&) = delete;

    ErrorString &operator=(ErrorString &&) = delete;

    explicit ErrorString() {
        bio_meth_.reset(BIO_meth_new(BIO_TYPE_SOURCE_SINK, "ErrorString"));
        BIO_meth_set_write(bio_meth_.get(), [](BIO *bio, const char *data, int length) {
            auto str = reinterpret_cast<std::string *>(BIO_get_data(bio));
            str->append(data, length);
            return length;
        });
        bio_.reset(BIO_new(bio_meth_.get()));
        if (!bio_) throw std::runtime_error("ErroString: error in BIO_new\n");
        BIO_set_data(bio_.get(), &str_);
        BIO_set_init(bio_.get(), 1);
    }

    BIO *bio() { return bio_.get(); }

    std::string str() &&{ return std::move(str_); }
};

[[noreturn]] void throw_runtme_error_openssl(const std::string &s) {
    ErrorString errorString;
    ERR_print_errors(errorString.bio());
    throw std::runtime_error(std::string("OpenSSl error: ") + s + '\n' + std::move(errorString).str());
}


inline UniqPtr<addrinfo> make_adrrinfo_ptr(const char *site, const char *port) {
    addrinfo hints{};
    addrinfo *res;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (int status = getaddrinfo(site, port, &hints, &res); status != 0)
        throw std::runtime_error(std::string("error in getaddrinfo: ") + gai_strerror(status));

    return UniqPtr<addrinfo>(res);
}

socket_fd::socket_fd(const std::string &host, const std::string &port) :
        addr_(make_adrrinfo_ptr(host.c_str(), port.c_str())),
        sock_fd(socket(addr_->ai_family, addr_->ai_socktype, addr_->ai_protocol)) {
    if (sock_fd < 0) throw std::system_error(errno, std::generic_category());
    if (connect(sock_fd, addr_->ai_addr, addr_->ai_addrlen) < 0)
        throw std::system_error(errno, std::generic_category());
}

socket_fd::~socket_fd() {
    shutdown(sock_fd, SHUT_RDWR);
    close(sock_fd);
}

void socket_fd::reload() {
    int new_sock_fd = socket(addr_->ai_family, addr_->ai_socktype, addr_->ai_protocol);
    if (new_sock_fd < 0) throw std::runtime_error("socket: creating socket error");
    if (connect(new_sock_fd, addr_->ai_addr, addr_->ai_addrlen) < 0)
        throw std::system_error(errno, std::generic_category());
    shutdown(sock_fd, SHUT_RDWR);
    close(sock_fd);
    sock_fd = new_sock_fd;
}

[[nodiscard]] int socket_fd::get() const {
    return sock_fd;
}

http_client::http_client() : sock_("api.vk.com", "443") {
    ctx_.reset(SSL_CTX_new(TLS_client_method()));
    if (!ctx_) {
        throw_runtme_error_openssl("error in SSL_CTX_new");
    }
    ssl_.reset(SSL_new(ctx_.get()));
    if (!ssl_) throw_runtme_error_openssl("error in SSL_new");
    SSL_set_fd(ssl_.get(), sock_.get());
    if (SSL_connect(ssl_.get()) != 1) throw_runtme_error_openssl("SSL_connection error");
}


inline std::string http_client::read_func() {
    char buffer[1024];
    int len = SSL_read(ssl_.get(), buffer, sizeof(buffer));
    if (len < 0) throw_runtme_error_openssl("SSL_read error");
    else if (len > 0) return std::string(buffer, len);
    else if (len == 0) throw_runtme_error_openssl("SSL_read: empty!");
}

inline std::string http_client::write_and_first_read_func(const std::string &req_) {
    SSL_write(ssl_.get(), req_.c_str(), req_.length());
    char buffer[1024];
    int len = SSL_read(ssl_.get(), buffer, sizeof(buffer));
    if (len < 0) throw_runtme_error_openssl("SSL_read error");
    else if (len > 0) return std::string(buffer, len);
    else if (len == 0) {
        reload();
        SSL_write(ssl_.get(), req_.c_str(), req_.length());
        len = SSL_read(ssl_.get(), buffer, sizeof(buffer));
        if (len < 0) throw_runtme_error_openssl("SSL_read error");
        else if (len > 0) return std::string(buffer, len);
        else if (len == 0) throw_runtme_error_openssl("SSL_read: is empty!!!");
    }
}

inline std::vector<std::string> http_client::lines_from_header(const std::string &str) {
    std::vector<std::string> out;
    std::size_t start{};
    std::size_t end;
    while ((end = str.find("\r\n", start)) != std::string::npos) {
        out.push_back(str.substr(start, end - start));
        start = end + 2;
    }
    return out;
}


std::string http_client::get_request(const std::string &req) {
    std::string header = write_and_first_read_func(req);
    std::size_t content_length{};
    auto end_pos = header.find("\r\n\r\n");
    while (end_pos == std::string::npos) {
        header += read_func();
        end_pos = header.find("\r\n\r\n");
    }
    for (auto &line : lines_from_header(header)) {
        if (auto pos = line.find(':'); pos != std::string::npos) {
            if (line.substr(0, pos) == "Content-Length")
                content_length = std::stoul(line.substr(pos + 1));
        }
    }
    std::string body(header, end_pos + 4);
    while (body.length() < content_length) {
        body += read_func();
    }
    header.resize(end_pos);
    return body;
}

void http_client::reload() {
    sock_.reload();
    ssl_.reset(SSL_new(ctx_.get()));
    if (!ssl_) throw_runtme_error_openssl("error in SSL_new");
    SSL_set_fd(ssl_.get(), sock_.get());
    if (SSL_connect(ssl_.get()) != 1) throw_runtme_error_openssl("SSL_connection error");
}





