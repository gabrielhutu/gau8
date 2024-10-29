#include "./connectors.hpp"
#include "./used_by_license_checker/sha256.h"

static std::string licenseBuffer;

static size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{ 
    size_t realsize = size * nmemb;
    licenseBuffer.append(contents, realsize);
    return realsize;
}


gau8::license::license(const char* user_id)
    : api(std::string(API_DOMAIN) + std::string("?") + std::string(user_id))
{
    curl_global_init(CURL_GLOBAL_SSL);
    this->handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, api.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteCallback);
}

gau8::license::~license()
{
    curl_easy_cleanup(this->handle);
    curl_global_cleanup();
}

bool check_key(const std::string& _key)
{
    
}


bool gau8::license::check_license()
{
    std::string return_data;
    curl_easy_perform(handle);
    return true;
}

