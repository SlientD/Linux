#include <iostream>
#include <httplib.h>
using namespace std;
using namespace httplib;




void func(const Request &req,Respond &resp){
    //req:http请求的内容
    //resp：需要响应的内容

    resp.set_content("<html>hello rdd</html>","123");
}

int main(){
   Server svr;

   svr.Get("/",func);
   svr.Get("/",[](const Request &req,Respond &resp){
            resp.set_content("<html>hello rdd</html>","123");
           });


   svr.listen();
   //参数1：ip地址 2.端口号




    return 0;
}
