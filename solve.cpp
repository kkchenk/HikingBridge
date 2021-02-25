#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <algorithm>
#include "yaml-cpp/yaml.h"

double bfsSolution(std::vector<double>& timeNeededPerHiker);

int main() {
  
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Bridge";
    out << YAML::Value<< YAML::BeginMap<<YAML::Key<<"Len"<<YAML::Value<<"100"<<YAML::Key<<"HikerSpeed"<<YAML::Value<< YAML::BeginSeq << 100<<50<<20<<10<< YAML::EndSeq<< YAML::EndMap;
    out << YAML::Key << "Bridge";
    out << YAML::Value<< YAML::BeginMap<<YAML::Key<<"Len"<<YAML::Value<<"250"<<YAML::Key<<"HikerSpeed"<<YAML::Value<< YAML::BeginSeq <<2.5<< YAML::EndSeq<< YAML::EndMap;
    out << YAML::Key << "Bridge";
    out << YAML::Value<< YAML::BeginMap<<YAML::Key<<"Len"<<YAML::Value<<"150"<<YAML::Key<<"HikerSpeed"<<YAML::Value<< YAML::BeginSeq <<25<<15<< YAML::EndSeq<< YAML::EndMap;
    out << YAML::Key << "Bridge";
    //out << YAML::Value<< YAML::BeginMap<<YAML::Key<<"Len"<<YAML::Value<<"150"<<YAML::Key<<"HikerSpeed"<<YAML::Value<< YAML::BeginSeq <<25<<15<< YAML::EndSeq<< YAML::EndMap;
    out << YAML::EndMap;

    std::ofstream outFile("./bridges.yaml");
    outFile<<out.c_str()<<std::endl;
    outFile.close();    

    YAML::Node bridges;
    try{
    	 bridges = YAML::LoadFile("./bridges.yaml");
    }
    catch(YAML::BadFile){
  	std::cout<<"Invalid input yaml file, quit"<<std::endl;
	return 0;
    }

    std::vector<double> hikingTeam;
    std::vector<double> currentHikers;
    double totalTime = 0;
    int bridgeCount = 0;
    double bridgeLength = 0;
  
    
    for(YAML::const_iterator it=bridges.begin(); it!=bridges.end(); ++it){
		
	const std::string &key=it->first.as<std::string>();
	currentHikers.clear();

        bridgeCount++;
	currentHikers.clear();
	
	YAML::Node attributes = it->second;

    	if(YAML::Node lenNode = attributes["Len"]){
            bridgeLength = lenNode.as<double>();
	}
	else{
	    std::cout<<"Bridge without length information encountered, move to the next"<<std::endl;
	    continue;
	}

	if(YAML::Node speeNode = attributes["HikerSpeed"]){
    	    for(int i=0; i<speeNode.size(); ++i)
      	        currentHikers.push_back(speeNode[i].as<double>());
    	}

	std::sort(currentHikers.begin(), currentHikers.end(), std::greater<double>());

	//get the list of hikers in non-increasing sorted order inside currentHikers
	if (bridgeCount == 1){//first bridge
	    hikingTeam = currentHikers;
	}
	else{// merge currentHikers and hikingTeam and keep result sorted in currentHikers
	    int lenC = currentHikers.size(), lenH = hikingTeam.size();
	    currentHikers.resize(lenC+lenH);
	    int i = lenC - 1, j = lenH - 1, k = lenC + lenH - 1;
	    while(i >= 0 || j >= 0){
		if (i < 0)
	            currentHikers[k--] = hikingTeam[j--];
		else if(j<0)
		    currentHikers[k--] = currentHikers[i--];
		else{
		    if(currentHikers[i] >= hikingTeam[j])
		        currentHikers[k--] = hikingTeam[j--];
		    else
		        currentHikers[k--] = currentHikers[i--];
		}
	    }
	
	}
	
        int count = currentHikers.size();
	std::vector<double> timeNeededPerHiker(count);
	for(int i = 0; i < count; i++){
	    timeNeededPerHiker[i] = bridgeLength / currentHikers[i];
	}

	double oneBridgeTime = 0;
	while (count > 0) {
	    if (count == 1) {
	        oneBridgeTime += timeNeededPerHiker[0];
	        count = 0;
	    }
	    else if (count == 2) {
	        oneBridgeTime += timeNeededPerHiker[1];
	        count = 0;
	    }
	    else if (count == 3) {
	        oneBridgeTime += timeNeededPerHiker[0] + timeNeededPerHiker[1] + timeNeededPerHiker[2];
	        count = 0;
	    }
	    else {

                oneBridgeTime += std::min(2 * timeNeededPerHiker[0] + timeNeededPerHiker[count - 2] + timeNeededPerHiker[count - 1],
					timeNeededPerHiker[0] + 2 * timeNeededPerHiker[1] + timeNeededPerHiker[count - 1]);
		count -= 2;
	    }
	}
	totalTime += oneBridgeTime;
	double crossValidation = bfsSolution(timeNeededPerHiker);
	std::cout << "The time needed at Bridge-bfs-method " << bridgeCount <<" (len: "<<bridgeLength<<") is " << crossValidation << " min" << std::endl;
	std::cout << "The time needed at Bridge-greedy-method " << bridgeCount <<" (len: "<<bridgeLength<<") is " << oneBridgeTime << " min" << std::endl;
	std::cout << "The total time used after Bridge " << bridgeCount<<" (len: "<<bridgeLength<<") is " << totalTime << " min" << std::endl<<std::endl;
    }
}


double bfsSolution(std::vector<double>& timeNeededPerHiker){
    int len = timeNeededPerHiker.size();
    std::string startState, endState;

    for(int i=0; i<len; i++){
    	startState.push_back('1');
	endState.push_back('0');
    }

    std::queue<std::string> q({startState});
    std::unordered_map<std::string, double> stateTimeMap;
    stateTimeMap[startState] = 0;

    int level = 0;
    while(!q.empty()){
        level++;
	int size = q.size();
        for(int i=0; i<size; i++){
	    auto cur = q.front(); q.pop();
            double curTime = stateTimeMap[cur];
            for(int m=0; m<len; m++){
                if(cur[m]=='1'){
                    for(int n=m+1;n<len; n++){
			if(cur[n] == '1'){
                            cur[m] = cur[n] ='0'; //m, n go
			    if(level == len-1){ //no need to back	
				if(stateTimeMap.count(cur))
				    stateTimeMap[cur] = std::min(stateTimeMap[cur], curTime+timeNeededPerHiker[n]);
				else{
				    stateTimeMap[cur] = curTime+timeNeededPerHiker[n];
				    q.push(cur);
				}		    
                            }
			    else{
			        for(int k=0; k<len; k++){//find one back 
				    if(cur[k] == '0'){
				        cur[k] = '1'; //k back
				        if(stateTimeMap.count(cur))
				       	    stateTimeMap[cur] = std::min(stateTimeMap[cur], curTime+timeNeededPerHiker[n]+timeNeededPerHiker[k]);
				        else{
					    stateTimeMap[cur] = curTime+timeNeededPerHiker[n]+timeNeededPerHiker[k];
					    q.push(cur);
				        }
					cur[k] = '0'; //restore
                                    }				    
                                }
			    }
			    cur[m] = cur[n] = '1' ;//restore
                        }
                    }
		}            
            }                    
        }
    }
    return stateTimeMap[endState];

}


