

#include "main_thread.h"




using namespace vk_parse;

vk_parse::main_thread::main_thread(const std::string &owner_id, const std::string &output_filename, int count_of_threads): 
owner_id_(std::move(owner_id)),
access_token_("2dfa071b2dfa071b2dfa071bde2d8ac66a22dfa2dfa071b737d6b938b670684ffa5a85e"),
//sql_writer(output_filename, owner_id ),
file(output_filename),
req_pool(count_of_threads)
{
    //sql_writer.create_new_group_table();
}

void vk_parse::main_thread::start(std::size_t count_of_posts){

    std::vector<std::future<std::string>> posts_id_futures;

    for ( std::size_t offset{}; offset < count_of_posts; offset+=100 ){

        posts_id_futures.emplace_back(req_pool.submit(
            [this, offset](simple_https::https_client &client)->std::string{

            return client.get_request("method/wall.get?count=100&access_token=" +
                                       access_token_ +
                                       "&v=5.110&owner_id=" +"-" +
                                       owner_id_ +"&offset=" +
                                       std::to_string(offset)
                                     );


        }));

    }

    std::cout << " post_futures " << posts_id_futures.size() << std::endl;

    for ( auto &post_fut: posts_id_futures ){

        if ( post_fut.wait_for(std::chrono::seconds(4)) == std::future_status::ready){

            auto posts = get_posts_ids(post_fut.get());

            for ( auto iter{posts.begin()}; iter != posts.end(); ){

                auto posts_start_iter{iter};
                std::vector<std::vector<std::future<std::string>>> likes_futures_vec; 

                for ( int i{}; i < 40 && iter != posts.end(); ++i, ++iter ){

                    posts_item post = *iter;
                    std::vector<std::future<std::string>> likes_futures;
                    int count{post[1]};

                    if ( post[1] == 0 ) {
                            std::cout << "post_is_empty" << std::endl;
                            continue;
                    }

                    for ( int offset = 0; count > 0; offset += 1000, count -= 1000 ) {
                        likes_futures.emplace_back(req_pool.submit([offset, this, post](simple_https::https_client &client)->std::string{

                            return client.get_request("method/likes.getList?type=post&count=1000&access_token=" +
                                                       access_token_ +
                                                       "&v=5.110&owner_id=" + "-"+
                                                       owner_id_ +
                                                       "&offset=" +
                                                       std::to_string(offset) +
                                                       "&item_id=" +
                                                       std::to_string(post[0])
                                                      );

                        }));
                    }

                    likes_futures_vec.emplace_back(std::move(likes_futures)); 
                }

                for ( auto likes_fut_vec_iter{likes_futures_vec.begin()} ;
                posts_start_iter <= iter && likes_fut_vec_iter != likes_futures_vec.end();
                 ++posts_start_iter, ++likes_fut_vec_iter )
                {

                    std::vector<std::uint32_t> output, temp;

                    for ( auto &likes_fut: *likes_fut_vec_iter ){

                        if ( likes_fut.wait_for(std::chrono::seconds(15)) == std::future_status::ready){
                            temp = get_likes_array(likes_fut.get());
                        } else {
                            if ( likes_fut.valid() ) std::cout << "valid!" << std::endl;
                            std::cout << "oops" << std::endl;
                            continue;
                        }

                        output.insert(output.end(), temp.begin(), temp.end());
                    }

                    std::cout << output.size()<<std::endl;
                    file.write(output, *posts_start_iter);
                    //sql_writer.write_likes_ids(output, *posts_start_iter);
                }

            }        
            
        } else {
            std::cout<<"oops 1" << std::endl;
        }
    }
}



