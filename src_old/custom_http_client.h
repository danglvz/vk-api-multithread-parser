//
// Created by dd on 19.06.2020.
//

#ifndef UNTITLED7_CUSTOM_HTTP_CLIENT_H
#define UNTITLED7_CUSTOM_HTTP_CLIENT_H
#include <iostream>
#include <openssl/ssl.h>
#include <netdb.h>
#include <memory>
#include <vector>


    template<typename TYPE>
    struct my_destructor;

    template<>
    struct my_destructor<BIO> {
        void operator()(BIO *bio) const { BIO_free_all(bio); }
    };

    template<>
    struct my_destructor<BIO_METHOD> {
        void operator()(BIO_METHOD *bio_meth) const { BIO_meth_free(bio_meth); }
    };

    template<>
    struct my_destructor<SSL> {
        void operator()(SSL *ssl) const { SSL_free(ssl); }
    };

    template<>
    struct my_destructor<SSL_CTX> {
        void operator()(SSL_CTX *ssl_ctx) const { SSL_CTX_free(ssl_ctx); }
    };

    template<>
    struct my_destructor<addrinfo> {
        void operator()(addrinfo *addrinfo_) const { freeaddrinfo(addrinfo_); }
    };

    template<typename TYPE>
    using UniqPtr = std::unique_ptr<TYPE, my_destructor<TYPE>>;

    class socket_fd {
        const UniqPtr<addrinfo> addr_;
        int sock_fd;
    public:
        explicit socket_fd(const std::string &host, const std::string &port);

        ~socket_fd();

        void reload();

        [[nodiscard]] int get() const;
    };


    class http_client {
        socket_fd sock_;
        UniqPtr<SSL_CTX> ctx_;
        UniqPtr<SSL> ssl_;

        inline std::string write_and_first_read_func(const std::string &req_);

        inline std::string read_func();

        inline std::vector<std::string> lines_from_header(const std::string &str);

    public:
        explicit http_client();

        std::string get_request(const std::string &);

        void reload();
    };




#endif //UNTITLED7_CUSTOM_HTTP_CLIENT_H
