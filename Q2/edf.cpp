#include <string.h>
#include <iostream>
#include <stdio.h>
#include <queue>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;
typedef pair<int, int> p;

void EDF(int size, string fileName)
{
    priority_queue<p, vector<p>, greater<p> > q;
    int arrive =0;
    int drop =0;
    int transmit = 0;
    int total = 0;
    //read from file
    ifstream openFile(fileName);
    string line;
    while (getline (openFile, line)) 
    {
        istringstream ss(line);
        string packet;
        //get one tuple at a time
        while (ss>> packet)
        {
            //remove the '(' ')' and split every number into the tuple
            string tempTuple = packet.substr(1,packet.size()-2);
            replace(tempTuple.begin(), tempTuple.end(), ',', ' ');
            stringstream anthorSS(tempTuple);
            //every tuple seems like that- (amount,slack,value)
            int amount, slack, value; 

            anthorSS >> amount >> slack >> value;
            
            //update the total arrived packets
            arrive += amount;
            //if slack and value are non zero or negative
            if(slack > 0 && value > 0)
            {
                //if there is more space in the queue push more packets
                while (q.size() < size && amount > 0)
                {
                    pair<int,int> p(slack, value);
                    q.push(p);
                    amount--;
                }  
             
                pair<int, int> check = q.top();
                //if the new packet have a bigger slack - drop the new packet
                while(check.first >= slack && amount >0)
                {
                    drop++; 
                    amount--;
                }
                /*if there is stil more packets to push to the queue
                so replace with the packet with minimun slack*/ 
                while(check.first< slack && amount>0)
                {   
                    //remove the packet with the minimum slack and push the new packet
                    q.pop();
                    pair<int,int> newPacket(slack, value);
                    q.push(newPacket);
                    amount--;
                    check = q.top();
                    drop++;
                }  
                //if we have more packet with bigger slack drop them
                while(amount >0 )
                {
                    drop++;
                    amount--;
                }
            }
            else{
                while (amount >0)
                {
                    drop++;
                    amount--;
                }
            }
        }
        
        priority_queue<p, vector<p>, greater<p> > help;
        //update the slack of every packet to be slack-1
        while(!q.empty())
        {
            pair<int,int> p= q.top();
            q.pop();
            p.first = p.first-1;
            if(p.first>0) help.push(p);
            else drop++; //if slack=0 drop the packet
        }
        q=help;

        /********processing********/
        //transmit the packet with the minimum slack
        if(!q.empty())
        {
            pair<int,int> toTransmit = q.top();
            q.pop();
            total += toTransmit.second;
            transmit++;
        }
    }

    /*there is no more packet to push to the queue, so we will countinue the processing
    until the queue will be empty*/
    while(!q.empty())
    {
        priority_queue<p, vector<p>, greater<p> > help;
        //update the slack of every packet to be slack-1
        while(!q.empty())
        {
            pair<int,int> p= q.top();
            q.pop();
            p.first = p.first-1;
            if(p.first>0) help.push(p);
            else drop++; //if slack=0 drop the packet
        }
        q=help;
        
        //transmit the packet with the minimum slack
        if(!q.empty())
        {
            pair<int,int> toTransmit = q.top();
            q.pop();
            total += toTransmit.second;
            transmit++;
        }
    }
    printf("Total arrived packets %d, total dropped packets %d, total transmitted packets %d, total transmitted value %d\n", arrive, drop, transmit, total);
}
    
int main(int argc, char** argv)
{
    int size = atoi(argv[1]);
    string fileName = argv[2];
    
    EDF(size, fileName);
}
