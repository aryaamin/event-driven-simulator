#include<bits/stdc++.h>
#include<unistd.h>
#include<queue>
using namespace std;



class packet{
public:

	int size;
	int comingfromindex;
	int goingtoindex;
	bool error=0;

	packet(int c,int g){
		size = rand()%950 + 50;
		comingfromindex = c;
		goingtoindex = g;
	}

};



class Node{
public:
	queue<packet> packlist;
	queue<packet> reclist;

	void packetgen(int c, int g){
	    packlist.push(packet(c,g));
        }
};



class Event{
public:

	Node *k;
	int type=0;  //0 for end, 1 for transmit, 2 for recieve 
	float time;

	Event(Node *n, float t, int ty){
		k=n;
		time=t;
		type = ty;
	}

	Event(float t){
		time =t;
	}
};



struct Comparetime{
    bool operator()(Event const& p1, Event const& p2)
    {
        // return "true" if "p1" is ordered
        // before "p2", for example:
        return p1.time > p2.time;
    }
};


int main(int argc, char **argv){

	int n=atoi(argv[1]);
	int t=atoi(argv[2]);
	srand(time(0));

	int period=10; //after every 10 sec create new packet for each node

	Node a[n]; //array with n nodes;

	priority_queue<Event, vector<Event>, Comparetime> q;

	//Event *packtransev[n];
	//Event *simuend = new Event(t);

	q.push(Event(t));

	for(int i=0; i<n; i++){

		int tim=rand()%(t/100);
		float time = tim;
		//packtransev[i] = new Event(&a[i], time);
		q.push(Event(&a[i], time, 1));
	}

    float sec=0;

    
    int ten = 0;

    vector<packet> onair;
 
	while(sec=sec+0.001){
       

		if(sec >= ten){
			for(int i=0; i<n; i++){
				int g = rand()%n;
				if(g==i){g++;}
				if(g==n){g=g-2;}
				a[i].packetgen(i,g);
			}
			ten=ten+period;
		}


        Event e=q.top();

        if(sec >= e.time){

       		q.pop();

       		if(e.type == 0){break;}

       		if(e.type == 1 && !e.k->packlist.empty()){


            	packet p1 = e.k->packlist.front();
            	e.k->packlist.pop();
            	float t = sec + p1.size / 1000.0;
     
            	q.push(Event(&a[p1.comingfromindex], t, 2)); //packet recieve event
            	onair.push_back(p1);
       

            	cout<<"Transmitting package from "<<p1.comingfromindex<<" to "<<p1.goingtoindex<<endl;

            	q.push(Event(&a[p1.comingfromindex], t+1, 1)); // next packet tranfer event(i have set it to 1sec after the recieve is done)
                


            }

            if(onair.size() > 1){
            	for(int i=0; i<onair.size(); i++){
            		onair[i].error = 1;
            	}
            }

            if(e.type == 2){

            	int cf,gt;
            	bool er;

            	for(int i=0; i<onair.size(); i++){

            		if(&a[onair[i].comingfromindex] == e.k){
            			cf=onair[i].comingfromindex;
            		    gt=onair[i].goingtoindex;
            		    er=onair[i].error;
            		    onair.erase(onair.begin()+i);
            		    break;
            		}

            	}



            	cout<<"Node "<<gt<<" recieved package from Node "<<cf;
            	if(er){cout<<" with error"<<endl;}
            	else{cout<<" without error"<< endl;}


            }

        }

		cout.flush();
		sleep(0.001);

	}
	cout<<sec;

}