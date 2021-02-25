# HikingBridge
1) How to compile

g++ -o solve solve.cpp -std=c++11 -I.  -L. -lyaml-cpp 

2) How to run

./solve

3) Solution strategy
Two solutions strategy are proposed here. 
a) Greedy solution:
First sort the time each hiker needs to cross the bridge in non-decreasing order.

If there are more than 4 hikers, each time try to send the two slowest hikers to the other side. Compare the time needed between two cases:
i) The two slowest hikers (s1, s2) go to together with the assitance of the two fastest hikers (f1, f2, they go together and be responsible for taking the torch back). The time needed here is f1+2*f2_s2
ii) The fastest hiker (f1) accompany the two slowest hikers (s1, s2) to cross the bridge and take the torch back. The time needed here is f1+s1+s2
Then, choose the case with the smaller time needed.

If there are 3 hikers (f1, f2, f3), the minimal time needed simply is f1+f2+f3

If there are 2 hikers (f1, f2, f3), the minimal time needed simply is f2;

If there is onely one hiker (f1), the minimal time needed simply is f1;

So greedily apply the above processing till all hikers cross the bridge. 


b) BFS solution:
First sort the time each hiker needs to cross the bridge in non-decreasing order.
We can think the process as send one hiker cross the bridge in each round (two cross the bridge and one take the torch back), except the last round in which two will cross the bridge.

We can use a string binary sequence to denote the state: 1111 means all four hikers are on the left of the bridge (which is the start state), and 0000 means all have crossed the bridge (which is the end state). 

In each round, we start from current state, say state_cur, find all pairs of hikers that are on the left of the bridge, send them to the right, then find one hiker on the right to take the torch back (except for the last round). This would lead to a new state, say state_new,  with one (or two in the last round) more hikers on the right of the bridge. Update the minimal time to reach the state_new based on the minimal time to reach state_cur. We use an unordered map of <string, int> to store the minimal time to reach a state. The return simply is the minimal state to reach the end state.  

4) Program architecture
a) Use the yaml-cpp lib to generate the input YAML file
A sample file is as the following
********************************
Bridge:
  Len: 100
  HikerSpeed:
    - 100
    - 50
    - 20
    - 10
Bridge:
  Len: 250
  HikerSpeed:
    - 2.5
Bridge:
  Len: 150
  HikerSpeed:
    - 25
    - 15
********************************
b) Use the yaml-cpp to read and parse the YAML file generated in step a)
c) Iterate all bridges as stored in the YAML file
d) For each bridge, apply the solution mentioned above. 

5) Edge case consideration
a) The input yaml file does not exist
b) The bridge information is incomplete in the input yaml file
c) The current code did not check whether the values are in the valid range, say length/speed >0. But it can easily added.

6) Testing
a) Both solutions are proposed to cross-validation the correctness of the program
b) Edge cases are tested




