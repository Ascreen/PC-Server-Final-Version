////////////////////////////////////////////////
////        PC 서버 통신, 좌표 계산 code     ///
////////////////////////////////////////////////

#include <winsock2.h>
#pragma comment(lib,"ws2_32")
#include <stdio.h>
#include <stdlib.h>


#define PORT 20000
#define LENGTH 5000


#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <math.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"

#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc.hpp"

#include "opencv/cxcore.h"

#include "opencv2/core/core.hpp"

#include "opencv2/imgproc/imgproc_c.h"

#include "opencv2/opencv.hpp"


//클릭지점 좌표값계산을 위한 
//
#define Pi 3.14159265
#define Length 510
#define Height 210

#define monitor_l 1440
#define monitor_h 900


//계산에 이용하는 배열 위치에 따른 데이터 정보
//
#define CameraNum 0
#define Time 1
#define X 2
#define Y 3


using namespace cv;
using namespace std;

int check_one=0, check_two=0, check_three=0, check_four=0;

//초기세팅값
//
Point dr_c(289,4);
Point dr_l(115,26);
Point dr_h(421,24);

Point ur_c(229,4);
Point ur_l(500,13);
Point ur_h(15,43);

Point ul_c(250,4);
Point ul_l(70,22);
Point ul_h(388,23);

Point dl_c(233,4);
Point dl_l(413,30);
Point dl_h(39,20);


//터치지점의 각도를 비교판단할 배열 
//
float dgree_dr[19] = {3.4614,
7.61169,
13.5178,
25.191,
47.6718,
86.6133,
118.745,
137.621,
146.52,
150.819,
154.547,
157.899,
160.238,
161.398,
162.426,
163.721,
164.143,
165.874};
float dgree_ur[19] = {2.4471,
3.77899,
5.27,
6.7812,
8.00009,
9.4223,
12.6147,
16.1583,
22.049,
29.6472,
45.1285,
74.9603,
110.31,
133.651,
150.111,
156.665,
161.75,
165.755};
 float dgree_ul[19] = {1.77769,
5.3857,
9.59779,
21.6321,
44.3095,
83.1209,
118.659,
139.234,
146.556,
152.052,
156.072,
158.473,
160.413,
161.86,
162.695,
163.659,
164.59,
166.57};
float dgree_dl[19] = {2.6597,
3.7518,
15.5269,
7.2778,
9.24471,
10.5619,
13.1965,
17.4465,
24.1444,
32.9833,
51.323,
82.4085,
117.983,
139.584,
154.232,
158.584,
162.249,
166.044};


// " void setting_parsing()" ; 4대의 각 카메라의 초기 기준 정보를 저장하는 함수 ; 영상 내 모니터 모서리 및 각 변의 중심점 정보
//
void setting_parsing(){
    string settingFilePath = "setting.txt";

    ifstream openFile(settingFilePath.data());
    if (openFile.is_open()){
        string line;
      int num = 0;
      while (getline(openFile, line)) {

         std::vector<char> writable(line.begin(), line.end());
         writable.push_back('\0');
         char* ptr = &writable[0];
         //std::cout << ptr;

         char* token = strtok(ptr, "/");

         int counting = 0;
         while (token) {
            if (counting == 0) {
               dr_c.x = atoi(token);
               counting++;
            }else if (counting == 1) {
               dr_c.y = Height-atoi(token);
               counting++;
            }else if (counting == 2) {
               dr_l.x = atoi(token);
               counting++;
            }else if (counting == 3) {
               dr_l.y = Height-atoi(token);
               counting++;
            }else if (counting == 4) {
               dr_h.x = atoi(token);
               counting++;
            }else if (counting == 5) {
               dr_h.y = Height-atoi(token);
               counting++;
            }else if (counting == 6) {
               ur_c.x = atoi(token);
               counting++;
            }else if (counting == 7) {
               ur_c.y = Height-atoi(token);
               counting++;
            }else if (counting == 8) {
               ur_l.x = atoi(token);
               counting++;
            }else if (counting == 9) {
               ur_l.y = Height-atoi(token);
               counting++;
            }else if (counting == 10) {
               ur_h.x = atoi(token);
               counting++;
            }else if (counting == 11) {
               ur_h.y = Height-atoi(token);
               counting++;
            }else if (counting == 12) {
               ul_c.x = atoi(token);
               counting++;
            }else if (counting == 13) {
               ul_c.y = Height-atoi(token);
               counting++;
            }else if (counting == 14) {
               ul_l.x = atoi(token);
               counting++;
            }else if (counting == 15) {
               ul_l.y = Height-atoi(token);
               counting++;
            }else if (counting == 16) {
               ul_h.x = atoi(token);
               counting++;
            }else if (counting == 17) {
               ul_h.y = Height-atoi(token);
               counting++;
            }else if (counting == 18) {
               dl_c.x = atoi(token);
               counting++;
            }else if (counting == 19) {
               dl_c.y = Height-atoi(token);
               counting++;
            }else if (counting == 20) {
               dl_l.x = atoi(token);
               counting++;
            }else if (counting == 21) {
               dl_l.y = Height-atoi(token);
               counting++;
            }else if (counting == 22) {
               dl_h.x = atoi(token);
               counting++;
            }else if (counting == 23) {
               dl_h.y = Height-atoi(token);
               counting++;
            }
            token = strtok(NULL, "/");
         }

      }
      openFile.close();
    }
}



float getAngle_from_plus(int x, int y){
    float result;
    result = atan2( y, x );
    result = result*180/Pi;
    if(x<0&&y<0){
        result = result*-1;
    }
    return result;
}


//"Point text_parsing(string, int)" ; 실시간으로 갱신되는 텍스트 파일로 부터 4대의 카메라에서 터치로 인식한 좌표정보를 계산할 변수에 저장한다.
//
Point text_parsing(string filePath, int camera){
    int tmp1[4];
    int tmp2[4];
    Point touched;
    ifstream openFile(filePath.data());
    if (openFile.is_open()){

        /******************************/
        string init;
        getline(openFile,init);
        //cout<<"init : "<<init<<endl;
        //cout<<init<<endl;
        std::vector<char> writable(init.begin(), init.end());
        writable.push_back('\0');
        char* init_tmp = &writable[0];
        char* token = strtok(init_tmp, "/");
        //cout<<token<<endl;
        tmp1[CameraNum] = atoi(token);
        token = strtok(NULL, "/");
        tmp1[Time]= atoi(token)*10000;
        token = strtok(NULL,"/");
        tmp1[Time]= tmp1[Time]+atoi(token)*100;
        token = strtok(NULL,"/");
        tmp1[Time]= tmp1[Time]+atoi(token);
        token = strtok(NULL,"/");
        tmp1[X] = atoi(token);
        token = strtok(NULL,"/");
        tmp1[Y] = atoi(token);
        /*****************************/
        int num = 1;
        string line;
        //cout<<"**************************"<<endl;
      while (getline(openFile, line)) {
        cout<<"line : "<<line<<endl;
         std::vector<char> writable(line.begin(), line.end());
         writable.push_back('\0');
         char* ptr = &writable[0];
            int time = 0;
         token = strtok(ptr, "/");
            tmp2[CameraNum] = atoi(token);
            token = strtok(NULL, "/");
            time= atoi(token)*10000;
            token = strtok(NULL,"/");
            time= time+atoi(token)*100;
            token = strtok(NULL,"/");
            time= time+atoi(token);
            tmp2[Time] = time;
            if(tmp2[Time]==0){
                break;
            }
            num++;
            token = strtok(NULL,"/");
            tmp2[X] = atoi(token);
            token = strtok(NULL,"/");
            tmp2[Y] = atoi(token);
            tmp1[Time] = ((tmp1[Time]*(num-1))+tmp2[Time])/num;
            tmp1[X] = ((tmp1[X]*(num-1))+tmp2[X])/num;
            tmp1[Y] = ((tmp1[Y]*(num-1))+tmp2[Y])/num;


        }

        touched.x = tmp1[X];
        touched.y = Height-tmp1[Y];
        cout<<"toched"<<endl;
        cout<<touched<<endl;
      openFile.close();
    }return touched;
}

//" float calculate_()" ; 이미지상의 좌표를 실제 윈도우의 픽셀값에 해당하도록 변환하는 함수
//
float calculate_(int camNum,Point touch){

     Point tmp_l(0,0);
     Point tmp_h(0,0);

     int angle = 0;
     float result = 0;

     if(camNum == 1){

        tmp_l.x = dr_l.x-dr_c.x;
        tmp_l.y = dr_l.y-dr_c.y;

        tmp_h.x = dr_h.x-dr_c.x;
        tmp_h.y = dr_h.y-dr_c.y;

        touch.x = touch.x-dr_c.x;
        touch.y = touch.y-dr_c.y;

        angle = getAngle_from_plus(tmp_l.x,tmp_l.y)-getAngle_from_plus(touch.x,touch.y);
        //cout<<getAngle_from_plus(tmp_l.x,tmp_l.y)-getAngle_from_plus(tmp_h.x,tmp_h.y)<<endl;
        //cout<<angle<<endl;
        for(int i = 0 ; i < 19 ; i++){
            if(dgree_dr[i]<angle){
                result = result+5;
            }
        }
        if(result==0){
            result=2.5;
        }
        else{
            result = result-2.5;
        }


        //cout<<"dr_l"<<dr_l<<endl;
    }else if(camNum ==2){

        tmp_l.x = ur_l.x-ur_c.x;
        tmp_l.y = ur_l.y-ur_c.y;

        tmp_h.x = ur_h.x-ur_c.x;
        tmp_h.y = ur_h.y-ur_c.y;

        touch.x = touch.x-ur_c.x;
        touch.y = touch.y-ur_c.y;

        angle = getAngle_from_plus(tmp_h.x,tmp_h.y)-getAngle_from_plus(touch.x,touch.y);
        //cout<<getAngle_from_plus(tmp_h.x,tmp_h.y)-getAngle_from_plus(tmp_l.x,tmp_l.y)<<endl;
        //cout<<angle<<endl;
        for(int i = 0 ; i < 19 ; i++){
            if(dgree_ur[i]<angle){
                result = result+5;
            }
        }
        if(result>90){
            result=2.5;
        }
        else{
        result = 90-(result-2.5);
        }

    }else if(camNum ==3){
        tmp_l.x = ul_l.x-ul_c.x;
        tmp_l.y = ul_l.y-ul_c.y;

        tmp_h.x = ul_h.x-ul_c.x;
        tmp_h.y = ul_h.y-ul_c.y;

        touch.x = touch.x-ul_c.x;
        touch.y = touch.y-ul_c.y;

        angle = getAngle_from_plus(tmp_l.x,tmp_l.y)-getAngle_from_plus(touch.x,touch.y);


        //cout<<getAngle_from_plus(tmp_l.x,tmp_l.y)-getAngle_from_plus(tmp_h.x,tmp_h.y)<<endl;
        //cout<<angle<<endl;
        for(int i = 0 ; i < 19 ; i++){
            if(dgree_ul[i]<angle){
                result = result+5;
            }
        }

        if(result==0){
            result=2.5;
        }else{
            result = result-2.5;
        }
        //cout<<"ul_l"<<ul_l<<endl;
    }else if(camNum ==4){

        tmp_l.x = dl_l.x-dl_c.x;
        tmp_l.y = dl_l.y-dl_c.y;

        tmp_h.x = dl_h.x-dl_c.x;
        tmp_h.y = dl_h.y-dl_c.y;

        touch.x = touch.x-dl_c.x;
        touch.y = touch.y-dl_c.y;

        angle = getAngle_from_plus(tmp_h.x,tmp_h.y)-getAngle_from_plus(touch.x,touch.y);
        //cout<<getAngle_from_plus(tmp_h.x,tmp_h.y)-getAngle_from_plus(tmp_l.x,tmp_l.y)<<endl;
        //cout<<angle<<endl;
        for(int i = 0 ; i < 19 ; i++){
            if(dgree_dl[i]<angle){
                result = result+5;
            }
        }
        if(result>90){
            result=2.5;
        }
        else{
        result = 90-(result-2.5);
        }
    }

    return result;

}

//" void clickHaJa" ; 삼각측량법을 이용해 터치한 지점의 좌표값으로 클릭이벤트 실행
//
void clickHaJa(Point dr,Point ur,Point ul,Point dl,int* click){
   float x;
    float y;

    float result1 = 0;
    float result2 = 0;
    double alpha;
    double beta;

    float dr_l_degree = 0;
    float dr_h_degree = 0;

    float ur_l_degree = 0;
    float ur_h_degree = 0;

    float ul_l_degree = 0;
    float ul_h_degree = 0;

    float dl_l_degree = 0;
    float dl_h_degree = 0;

    if(click[0]==1){
        dr_l_degree=calculate_(1,dr);
        dr_h_degree=90-dr_l_degree;

        cout<<"dr_l :"<<dr_l_degree<<endl;


    }if(click[1]==1){
        ur_l_degree = calculate_(2,ur);
        ur_h_degree=90-ur_l_degree;
      cout<<"ur_l :"<<ur_l_degree<<endl;


    }if(click[2]==1){
        ul_l_degree = calculate_(3,ul);
        ul_h_degree=90-ul_l_degree;
        cout<<"ur_l :"<<ul_l_degree<<endl;



    }if(click[3]==1){
        dl_l_degree = calculate_(4,dl);
        dl_h_degree=90-dl_l_degree;
        cout<<"dl_l :"<<dl_l_degree<<endl;

    }

    cout<<"clicked with!"<<dr<<ur<<ul<<dl<<endl;

        if(ur_h_degree != 0 && dr_h_degree != 0){
            alpha = ur_h_degree*Pi/180;
            beta = dr_h_degree*Pi/180;
            result1 = sin(alpha)*sin(beta)*monitor_h/sin(Pi-alpha-beta);

            if(result1<0){
                result1 = result1*(-1);

            }
            result1 = monitor_l-result1;

    }

        if(ul_h_degree != 0 && dl_h_degree != 0){

            alpha = ul_h_degree*Pi/180;
            beta = dl_h_degree*Pi/180;

            result2 = sin(alpha)*sin(beta)*monitor_h/sin(Pi-alpha-beta);

            if(result2<0){
            result2 = result2*(-1);
            }


    }
    if(result1!=0&&result2!=0){
        x = (result1+result2)/2;
    }
    else if (result1 == 0){
        x =result2;
    }
    else if (result2 == 0){
        x = result1;
    }
    cout<<"x:"<<x<<endl;
    result1=0;
    result2=0;
    alpha=0;
    beta=0;

    if(ur_l_degree != 0 && ul_l_degree != 0){
            alpha = ur_l_degree*Pi/180;
            beta = ul_l_degree*Pi/180;
            result1 = abs(sin(alpha)*sin(beta)*monitor_l/sin(Pi-alpha-beta));


    }
    if(dr_l_degree != 0 && dl_l_degree != 0){
            alpha = dr_l_degree*Pi/180;
            beta = dl_l_degree*Pi/180;
            result2 = abs(sin(alpha)*sin(beta)*monitor_l/sin(Pi-alpha-beta));
            result2 = monitor_h - result2;

    }

    if(result1!=0&&result2!=0){
       y = (result1+result2)/2;

    }
    else if (result1 == 0){
        y = result2;
    }
    else if (result2 == 0){
        y = result1;
    }
    cout<<"y:"<<y<<endl;




    float x_plot_center[] = {447.5,582.5,717.5,852.5,987.5};
    float y_plot_center[] = {177.5,312.5,447.5,582.5,717.5};

    x = abs(x);
    if(x>720){
            x = x*0.92;
        if(x>850){
            x = x*0.93;
            if(x>1000){
                x = x*0.96;
                while(x>1440){
                    x = x*0.85;
                }
            }
        }
    }


    if(x<720){
        x = x*1.06;
        if(x<540){
            x = x*1.07;
            if(x<380){
                x = x*1.12;
                if(x<300){
                    x = x*1.5;
                    if(x<230){
                        x = x*1.7;
                        if(x<150){
                            x = x*1.8;
                        }
                    }
                }
            }
        }
    }


    while((380-x)>0&&(380-x)<75){
        x = x+50;
    }
    if(abs(y)<225){y = abs(y);}
    if(y<-225){y = 160;}
    if(y>225&&y<430){
        y = y*1.2;
    }
    if(470<y){
        y = y*0.93;
        if(675<y){
            y = y*0.92;
            if(785<y){
                y = 720;

            }
        }
    }




    //x = (int)((monitor_l*((int)x/X_plot+1)/7)+(monitor_l*((int)x/X_plot)/7))/2;


    if(x != 0 && y != 0){
        SetCursorPos(x,y);
        mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
        mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
        cout<<"clicked:["<<x<<","<<y<<"]"<<endl;
        //Sleep(1000);
    }
}


//"void clickHalKa()" ; 터치를 인식한 카메라가 3대이상 존재하는지 검사 후 해당할 때 함수 clickHaJa실행
//
void clickHalKa(Point dr,Point ur,Point ul,Point dl){
    int click[4] = {0,0,0,0};
    if(dr.x!=0&&dr.y!=210){click[0]++;};
    if(ur.x!=0&&ur.y!=210){click[1]++;};
    if(ul.x!=0&&ul.y!=210){click[2]++;};
    if(dl.x!=0&&dl.y!=210){click[3]++;};

    if(click[0]+click[1]+click[2]+click[3]>2){
    clickHaJa(dr,ur,ul,dl,click);
    }
        //cout<<"why...?"<<endl;
}

//"int click_main()" ; 통신을 통해 새 데이터를 전달받을때마다 실행한다.
//

int click_main() {
    //setting_parsing();
    Point dr_touched(0,0);
    Point ur_touched(0,0);
    Point ul_touched(0,0);
    Point dl_touched(0,0);

    cout<<"---------dr----------"<<endl;
   dr_touched=text_parsing("C:/Users/ksy/Desktop/one.txt", 1);
   cout<<"---------ur----------"<<endl;
   ur_touched=text_parsing("C:/Users/ksy/Desktop/two.txt", 2);
   cout<<"---------ul----------"<<endl;
   ul_touched=text_parsing("C:/Users/ksy/Desktop/three.txt", 3);
   cout<<"---------dl----------"<<endl;
   dl_touched=text_parsing("C:/Users/ksy/Desktop/four.txt", 4);

    clickHalKa(dr_touched,ur_touched,ul_touched,dl_touched);

   return 0;
}


int main(int argc, char** argv)
{

	WSADATA wsaData;	 //	초기화 //
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	char revbuf[LENGTH]; // Receive buffer //
	int success = 0;
	int szClntAddr;



	if (argc != 2) {	//	포트정보가 입력되었는지 여부 검사	//
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	//	윈속 2.2사용 초기화	//
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)	//	Error check	//
		exit(1);
	else
		printf("[Server] Winsock2.2 Configuring is successfully\n");

	//	소켓을 생성 //
	hServSock = socket(AF_INET, SOCK_STREAM, 0);	//	TCP socket	//
	if (hServSock == INVALID_SOCKET)
		exit(1);
	else
		printf("[Server] Creating socket successfully\n");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;			//	TCP를 사용 //
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);	//	모든 IP를 받음	//
	servAddr.sin_port = htons(atoi(argv[1]));	//	포트 정보 //
	

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)	//	소켓에 주소를 부여 //
		exit(1);
	else

		printf("[Server] Binded tcp port %d in addr host successfully.\n", PORT);

	if (listen(hServSock, 10) == SOCKET_ERROR)	//	소켓대기상태, 동시에 10명까지 가능 	//
		exit(1);
	else
		printf("[Server] Listening the port %d successfully.\n", PORT);

	while (success == 0)
	{
		szClntAddr = sizeof(SOCKADDR_IN);

		// Wait a connection //
		
		if ((hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr)) == SOCKET_ERROR)
		{
			fprintf(stderr, "ERROR: Obtaining new Socket Despcritor. (errno = %d)\n", errno);
			exit(1);
		}
		else{
            printf("[Server] Server has got connected from Client.\n");
		}

		
        //	Receive Data from Client(save to .txt file)	//
        
        
        const char* fr_one = "C:/Users/ksy/Desktop/one.txt";
        const char* fr_two = "C:/Users/ksy/Desktop/two.txt";
        const char* fr_three = "C:/Users/ksy/Desktop/three.txt";
        const char* fr_four = "C:/Users/ksy/Desktop/four.txt";


        FILE *fr;
        memset(revbuf, LENGTH, '0');
        int fr_block_sz = 0;
        //일정 Block size로 data 전체 읽기, 어느 라즈베리파이의 data인지 확인 후 저장
        while ((fr_block_sz = recv(hClntSock, revbuf, LENGTH, 0)) > 0)
        {
            if (!strncmp(&revbuf[0],"1",1)) {
                fr = fopen(fr_one, "w");
                cout<<"one"<<endl;
                check_one = 1;
            }
            else if (!strncmp(&revbuf[0], "2",1)) {
                fr = fopen(fr_two, "w");
                cout<<"two"<<endl;
                check_two = 1;
            }
            else if (!strncmp(&revbuf[0], "3",1)) {
                fr = fopen(fr_three, "w");
                check_three = 1;
                cout<<"three"<<endl;
            }
            else {
                fr = fopen(fr_four, "w");
                check_four = 1;
                cout<<"four"<<endl;
            }
            int write_sz1 = fwrite(revbuf, sizeof(char), fr_block_sz, fr);

            if (write_sz1 < fr_block_sz)
            {
                exit(1);
            }

            memset(revbuf, LENGTH, '0');
            fclose(fr);

        }

        //4개 Pi의 data 모두 다 써져있는지 여부 확인//
        if (check_one + check_two + check_three + check_four == 4) {
            cout<<"start!!"<<endl;

            click_main();
            check_one = 0;
            check_two = 0;
            check_three = 0;
            check_four = 0;
       }

	}
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}