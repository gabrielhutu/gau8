#include "./connectors.hpp"

static std::string buffer;

gau8::license::license(const char* user_id)
    : api(std::string(API_DOMAIN) + std::string("?") + std::string(user_id))
{
    curl_global_init(CURL_GLOBAL_SSL);
    this->handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, api.c_str());
}

gau8::license::~license()
{
    curl_easy_cleanup(this->handle);
    curl_global_cleanup();
}

bool gau8::license::check_license()
{
    std::string return_data;
    curl_easy_perform(handle);
    return true;
}