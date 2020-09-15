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
        
void BD(int size, string fileName)
{
    priority_queue<p, vector<p>, greater<p> > q;
    int arrive =0;
    int drop =0;
    int transmit = 0;
    int total = 0;
    string line;
    ifstream openFile(fileName);
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
            stringstream anotherSS(tempTuple);
            //every tuple seems like that- (amount,slack,value)
            int amount, slack, value;

            anotherSS >> amount >> slack >> value;
            
            //update the total arrived packets
            arrive += amount;
            //if slack and value are non zero or negative
            if(slack > 0 && value >0)
            {
                //if there is more space in the queue push more packets
                while (q.size() < size && amount > 0)
                {
                    pair<int,int> newPacket(value  , slack);
                    q.push(newPacket);
                    amount--;
                }

                pair<int, int> check = q.top();
                //if the new packet have a small value - drop the new packet
                while(check.first >= value && amount >0) 
                {
                    drop++; 
                    amount--;
                }
                /*if there is stil more packets to push to the queue
                so replace with the packet with minimun value*/  
                while(check.first< value && amount>0)
                {   
                    //remove the packet with the minimum value and push the new packet
                    q.pop();
                    pair<int,int> newPacket(value, slack);
                    q.push(newPacket);
                    amount--;
                    check = q.top();
                    drop++;
                }  
                //if we have more packet with small value - drop them
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

        priority_queue<p, vector<p>, greater<p> > help_queue;
        //update the slack of every packet to be slack-1
        while(!q.empty())
        {
            pair<int,int> newPacket= q.top();
            q.pop();
            newPacket.second = newPacket.second-1;
            if(newPacket.second>0) help_queue.push(newPacket);
            else drop++; //if slack=0 drop the packet
        }
        q=help_queue;

        priority_queue<p, vector<p>, greater<p> > help;
        /********processing********/
        //transmit the packet with the maximum value
        while(q.size() > 1)
        {
            help.push(q.top());
            q.pop();
        }
        if(!q.empty())
        {
            pair<int,int> toTransmit = q.top();
            q.pop();
            total += toTransmit.first;
            transmit++;
            q = help;
        }
    }
    
    /*there is no more packet to push to the queue, so we will countinue the processing
    until the queue will be empty*/
    while(!q.empty())
    {
        priority_queue<p, vector<p>, greater<p> > help_queue;
        //update the slack of every packet to be slack-1
        while(!q.empty())
        {
            pair<int,int> newPacket= q.top();
            q.pop();
            newPacket.second = newPacket.second-1;
            if(newPacket.second>0) help_queue.push(newPacket);
            else drop++; //if slack=0 drop the packet
        }
        q=help_queue;

        priority_queue<p, vector<p>, greater<p> > help;
        //transmit the packet with the maximum value
        while(q.size() > 1)
        {
            help.push(q.top());
            q.pop();
        }
        if(!q.empty())
        {
            pair<int,int> toTransmit = q.top();
            q.pop();
            total += toTransmit.first;
            transmit++;
            q = help;
        }
    }  
    printf("Total arrived packets %d, total dropped packets %d, total transmitted packets %d, total transmitted value %d\n", arrive, drop, transmit, total);
}
       
int main(int argc, char** argv)
{    
    int size = atoi(argv[1]);
    string file = argv[2];
    
    BD(size, file);
}
