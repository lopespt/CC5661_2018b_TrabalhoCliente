#ifndef COMMUNICATIONS_HPP
#define COMMUNICATIONS_HPP

#include <array> 
#include "json.hpp"
#include <curl/curl.h>
#include <string>
#include <sstream>
#include <iostream>
#include <functional>

using namespace std;
using namespace nlohmann;



size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata){
    string* s = (string*)(userdata);
    s->append(ptr,nmemb);
    return nmemb;
}


json receive_problem(string url,int idGrupo){
        string buffer_s="";
        url.append("?idGrupo=").append(to_string(idGrupo));
        CURL *curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer_s);
        CURLcode res = curl_easy_perform(curl);
        json j;
        if(res != CURLE_OK){
            cout << "Erro recebido do servidor" << endl;
        }else{
            j = json::parse(buffer_s);
        }
        curl_easy_cleanup(curl);
        return j;
}
size_t ignore_func(void *buffer, size_t size, size_t nmemb, void *userp)
{
   return size * nmemb;
}
void send_solution(string url, json s){
        struct curl_slist *headers = NULL;
        CURL *curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        string stringfied = s.dump();
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, stringfied.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, stringfied.length());
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ignore_func);
        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK){
            cout << "Erro recebido do servidor" << endl;
        }
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
}



void connect(string url, int idGrupo, function<json(json, unsigned int)> func){
    unsigned int numProb=0;
    while(1){
        json j = receive_problem(url, idGrupo);
        json prob = j["problema"];
        json resp;
        array<string,3> tipos={"grafo", "ordenacao", "multiplicacao_matrizes"};

        bool is_prob = any_of(tipos.begin(), tipos.end(), [&prob](string x){return x==prob["tipo"].get<string>();});
        if(is_prob){
            resp["solucao"] = func(prob,numProb++);
            resp["uuid"] = j["uuid"];
            send_solution(url+"/solucao", resp);
        }else{
            //command(j)
            cout << "is command" << endl;
        }
        
        //return json::parse(resp_json);
    }
}

#endif