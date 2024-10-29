#include "./connectors.hpp"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

static std::string licenseBuffer;

static size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{ 
    size_t realsize = size * nmemb;
    licenseBuffer.append(contents, realsize);
    return realsize;
}


gau8::license::license()
    : api(std::string(API_DOMAIN))
{
    curl_global_init(CURL_GLOBAL_SSL);
    this->handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, api.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteCallback);
    //Using the useragent as the get request param for the user ID 
    curl_easy_setopt(handle, CURLOPT_USERAGENT, USER_ID);
}

gau8::license::~license()
{
    curl_easy_cleanup(this->handle);
    curl_global_cleanup();
}


std::string sha256(const std::string &str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << std::setw(2) << std::setfill('0') << std::hex << (int)hash[i];
    }
    return ss.str();
}



bool gau8::license::check_license()
{
    //Calc the hash of the user secret 
    std::string local_hash = sha256(std::to_string(SECRET));
    //Send the GET request to the API
    curl_easy_perform(handle);
    //Cut off the header and the trailer from the fetched key, then compare them and return the result
    return local_hash == licenseBuffer.substr(START_KEY_INDEX, 64);
}

