#include <bits/stdc++.h>
using namespace std;

#define maximum_nodes 50
#define maximum_conn 100

FILE *output;
void readInputTopology(char top_filename[],long double network_topology[][maximum_nodes][2],int* number_of_nodes,int* number_of_edges)
{
  cout<<"input file name is "<<top_filename<<endl;
  FILE *ip;
  ip = fopen(top_filename,"r");
  int source,dest;
  long double linkcost,bandwidth;
  if(!ip){
     cout<<"please provide input file name"<<endl;
     exit(0);
  }

  fscanf(ip,"%d %d",number_of_nodes,number_of_edges);
  cout<<"nodes = "<<*number_of_nodes<<" edges = "<<*number_of_edges<<endl;
  for(int i=0;i<*number_of_edges;i++){
    fscanf(ip,"%d %d %Lf %Lf",&source,&dest,&linkcost,&bandwidth);
    network_topology[source][dest][0] = network_topology[dest][source][0] = linkcost;
    network_topology[source][dest][1] = network_topology[dest][source][1] = bandwidth;
  }
  fclose(ip);

  return;
}

void assignCostParent(long double cost_parent[][maximum_nodes][2],long double network_topology[][maximum_nodes][2],int num_of_nodes,char type_of_ip[]){

   for(int i=0;i<num_of_nodes;i++){
     for(int j=0;j<num_of_nodes;j++){

         if(i==j){
             cost_parent[i][j][0] = 0;
             cost_parent[i][j][1] = -1;
         }
         else if(network_topology[i][j][0]!=0){

             if(strcmp(type_of_ip,"hop")==0){
                cost_parent[i][j][0] = 1;
             }
             else
                cost_parent[i][j][0] = network_topology[i][j][0];
             cost_parent[i][j][1] = i;
         }
         else{
            cost_parent[i][j][0] = INT_MAX;
            cost_parent[i][j][1] = -1;
         }

     }
   }
}

void allPairsShortestPath(long double cost_parent[][maximum_nodes][2],int number_of_nodes){

    int i, j, k;
    for (k = 0; k < number_of_nodes; k++)  // when vertex k acts as an intermediate node
    {
        // current source node = i
        for (i = 0; i < number_of_nodes; i++)
        {
            // check all vertices from vertex i
            for (j = 0; j < number_of_nodes; j++)
            {
                // If vertex k is on the shortest path from i to j, then update the value of cost_parent[][][2];
                if (cost_parent[i][k][0]!=INT_MAX && cost_parent[k][j][0]!=INT_MAX && cost_parent[i][k][0] + cost_parent[k][j][0] < cost_parent[i][j][0])
                {
                    cost_parent[i][j][0] = cost_parent[i][k][0] + cost_parent[k][j][0];
                    cost_parent[i][j][1] = cost_parent[k][j][1];
            }
        }
    }
}
}


void storeShortestPath(long double cost_parent[][maximum_nodes][2],int number_of_nodes,long double first_shortest_path[][maximum_nodes][maximum_nodes],long double first_second[][maximum_nodes][2]){
int hop_count;
   for(int i = 0; i < number_of_nodes; i++)
	{
		for(int j = 0; j < number_of_nodes; j++)
		{
		   hop_count = 0;
           if(i==j){
               first_shortest_path[i][j][0] = -1; // -100 can consider as self loop
           }
           else{
               int first,second;
               first = j;
               second = i;
               while(cost_parent[first][second][1]!=-1){
                    first_shortest_path[i][j][hop_count] = cost_parent[first][second][1];
                    hop_count++;
                    second = cost_parent[first][second][1];
               }
               first_shortest_path[i][j][hop_count] = -1;
           }
           first_second[i][j][0] = cost_parent[i][j][0];
		}

    }
}

void removePreviousShortestPath(long double first_shortest_path[][maximum_nodes][maximum_nodes],long double cost_parent[][maximum_nodes][2],int src,int dest){

    int hop = 0;
    int i = src,j = first_shortest_path[src][dest][hop];

    while(first_shortest_path[src][dest][hop]!=-1){

       cost_parent[i][j][0] = INT_MAX;
       cost_parent[j][i][0] = INT_MAX;
       cost_parent[i][j][1] = -1;
       cost_parent[j][i][1] = -1;
       i = j;
       hop++;
       j = first_shortest_path[src][dest][hop];

    }
}

void storeSecondShortestPath(long double cost_parent[][maximum_nodes][2],long double sec_shortest_path[][maximum_nodes][maximum_nodes],int i,int j,long double first_Second[][maximum_nodes][2],long double network_topology[][maximum_nodes][2]){

   int first,second,hop_count;
   hop_count = 0;
   first = j;
   second = i;
   long double cost=0;
   while(cost_parent[first][second][1]!=-1){
        sec_shortest_path[i][j][hop_count] = cost_parent[first][second][1];
        hop_count++;
        second = cost_parent[first][second][1];
   }
   first_Second[i][j][1] = cost_parent[i][j][0];
   sec_shortest_path[i][j][hop_count] = -1;
}


void findSecondShortestPath(long double network_topology[][maximum_nodes][2],long double cost_parent[][maximum_nodes][2],int num_of_nodes,long double sec_shortest_path[][maximum_nodes][maximum_nodes],long double first_shortest_path[][maximum_nodes][maximum_nodes],char type_of_ip[],long double first_second[][maximum_nodes][2])
{
   for(int i=0;i<num_of_nodes;i++){

      for(int j=0;j<num_of_nodes;j++){

           if(i!=j){

              removePreviousShortestPath(first_shortest_path,cost_parent,i,j);
              allPairsShortestPath(cost_parent,num_of_nodes);
              storeSecondShortestPath(cost_parent,sec_shortest_path,i,j,first_second,network_topology);
              assignCostParent(cost_parent,network_topology,num_of_nodes,type_of_ip);
           }
           else{
              sec_shortest_path[i][j][0] = -1;
           }
      }
   }
}

void storeRoutingInfoInFile(char filename[],long double first_shortest_path[][maximum_nodes][maximum_nodes],long double sec_shortest_path[][maximum_nodes][maximum_nodes],int num_of_nodes,char type_of_ip[],long double net_topology[][maximum_nodes][2]){

  FILE *fp;
  fp = fopen(filename,"w");
  for(int i=0;i<num_of_nodes;i++){
    fprintf(fp,"\n\nsource node = %d\n\n",i);
    for(int j=0;j<num_of_nodes;j++){
       //store first shortest path
       fprintf(fp,"destination node = %d ",j);
       fprintf(fp,"\t\t %d",i);
       int hop = 0,pathCost = 0;
       if(strcmp(type_of_ip,"hop")==0){

           while(first_shortest_path[i][j][hop]!=-1){
                 fprintf(fp,"-%d",(int)first_shortest_path[i][j][hop]);
                 hop++;
                 pathCost++;
           }
       }
       else{
         int s,d;
         s = i;
         d = first_shortest_path[i][j][hop];
         while(first_shortest_path[i][j][hop]!=-1){
               fprintf(fp,"-%d",(int)first_shortest_path[i][j][hop]);
               pathCost += net_topology[s][d][0];
               hop++;
               s = d;
               d = first_shortest_path[i][j][hop];
        }
       }
       fprintf(fp,"\t\t");
       fprintf(fp,"%d\n",pathCost);

        //store second shortest path
       fprintf(fp,"destination node = %d ",j);
       fprintf(fp,"\t\t %d",i);
       hop = 0;
       pathCost = 0;
       if(strcmp(type_of_ip,"hop")==0){

           while(sec_shortest_path[i][j][hop]!=-1){
                 fprintf(fp,"-%d",(int)sec_shortest_path[i][j][hop]);
                 pathCost++;
                 hop++;
           }
       }
       else{
         int s,d;
         s = i;
         d = sec_shortest_path[i][j][hop];
         while(sec_shortest_path[i][j][hop]!=-1){
               fprintf(fp,"-%d",(int)sec_shortest_path[i][j][hop]);
               pathCost += net_topology[s][d][0];
               hop++;
               s = d;
               d = sec_shortest_path[i][j][hop];
        }
       }
       fprintf(fp,"\t\t");
       fprintf(fp,"%d\n\n\n",pathCost);




    }
  }
  fclose(fp);

}

void readConnectionsFile(char conn_filename[],int connections[][5],int *conn_count){

   FILE *ip;
   ip = fopen(conn_filename,"r");
   if(!ip){
      cout<<"\nplease input connections file, some error is there\n";
      exit(0);
   }
   fscanf(ip,"%d",conn_count);

   for(int i=0;i<*conn_count;i++){
      fscanf(ip,"%d %d %d %d %d",&connections[i][0],&connections[i][1],&connections[i][2],&connections[i][3],&connections[i][4]);
   }
   fclose(ip);
   cout<<"connections file first line is "<<connections[0][0]<<" "<<connections[0][1]<<endl;
}

void assignLabels(long double network_topology[][maximum_nodes][2],int edge_count,int label[][maximum_nodes],int node_count){
  int start = 200;
  for(int i=0;i<node_count;i++){
     for(int j=0;j<node_count;j++){
        if(network_topology[i][j][0]!=0 && label[i][j]==-1){
               label[i][j] = label[j][i]=start;
               start+=10;
        }
     }
  }
}

void assignInterfaces(int label[][maximum_nodes],int node_count,int interface[][maximum_nodes]){
   int temp;
   for(int i=0;i<node_count;i++){
      temp = 1;
      for(int j=0;j<node_count;j++){
         if(label[i][j]!=-1){
             interface[i][j]=temp;
             temp++;
         }
      }
   }
}

long double computeMin(long double a,long double b){

   if(a<=b){
      return a;
   }
   else{
      return b;
   }

}

bool conditionToMet(long double fir_spath[][maximum_nodes][maximum_nodes],int src,int dest,long double net_top[maximum_nodes][maximum_nodes][2],long double beq){
   bool res = false;
   int curr,next,hop=0;
   curr = src;
   next = fir_spath[src][dest][hop];
   while(next!=-1){
     cout<<"curr = "<<curr<<" next= "<<next<<endl;
     if(net_top[curr][next][1]<beq){
        return false;
     }
     curr = next;
     hop++;
     next = fir_spath[src][dest][hop];
   }
   cout<<"checking basic req completed"<<endl;
   hop = 0;
   curr = src;
   next = fir_spath[src][dest][hop];
   while(next!=-1){
     net_top[curr][next][1] -= beq;
     net_top[next][curr][1] -= beq;
     curr = next;
     hop++;
     next = fir_spath[src][dest][hop];
   }

   return true;

}

void updateForwardingTable(int forwardingtable[][maximum_conn][4],long double fir_spath[][maximum_nodes][maximum_nodes],int node_count,int label[][maximum_nodes],int interface[][maximum_nodes],int src,int dest,int conn_id){
   conn_id+=1;
   cout<<"source = "<<src<<"  ,destination = "<<dest<<endl;
   fprintf(output,"%d \t %d   %d\t\t\t[",conn_id,src,dest);
   int curr,next,prev,hop=0;
   curr = src;
   prev = src;
   next = fir_spath[src][dest][hop];
   while(curr!=-1){
      if(curr==src){
         fprintf(output,"- %d",label[curr][next]);
         int next_slot_to_fill = 0;
         while(forwardingtable[curr][next_slot_to_fill][0]!=-1){
            next_slot_to_fill++;
         }
         forwardingtable[curr][next_slot_to_fill][0] = -100;
         forwardingtable[curr][next_slot_to_fill][1] = -100;
         forwardingtable[curr][next_slot_to_fill][2] = interface[curr][next];
         forwardingtable[curr][next_slot_to_fill][3] = label[curr][next];
         prev = curr;
         curr = next;
         hop++;
         next = fir_spath[src][dest][hop];
      }
      else if(curr==dest){
         int next_slot_to_fill = 0;
         while(forwardingtable[curr][next_slot_to_fill][0]!=-1){
           next_slot_to_fill++;
         }
         forwardingtable[curr][next_slot_to_fill][0] = interface[dest][prev];
         forwardingtable[curr][next_slot_to_fill][1] = label[dest][prev];
         forwardingtable[curr][next_slot_to_fill][2] = -100;
         forwardingtable[curr][next_slot_to_fill][3] = -100;
         hop++;
         prev = curr;
         curr = next;
         next = fir_spath[src][dest][hop];
      }
      else{
          fprintf(output,"- %d",label[curr][next]);
          int next_slot_to_fill = 0;
          while(forwardingtable[curr][next_slot_to_fill][0]!=-1){
            next_slot_to_fill++;
          }
         forwardingtable[curr][next_slot_to_fill][0] = interface[curr][prev];
         forwardingtable[curr][next_slot_to_fill][1] = label[curr][prev];
         forwardingtable[curr][next_slot_to_fill][2] = interface[curr][next];
         forwardingtable[curr][next_slot_to_fill][3] = label[curr][next];
         prev = curr;
         curr = next;
         hop++;
         next = fir_spath[src][dest][hop];
      }
   }
}

void checkConnections(int forwardingtable[][maximum_conn][4],int label[][maximum_nodes],int interface[][maximum_nodes],int nodes,char o_or_p,char ft_fn[],char p_fn[],int connections[][5],int con_num, long double fir_spath[][maximum_nodes][maximum_nodes], long double sec_spath[][maximum_nodes][maximum_nodes],long double net_top[][maximum_nodes][2],long double first_second[][maximum_nodes][2],char paths_filename[]){
   output = fopen("output.txt","w");
   int src,dest,hop,prev,curr,next,conn_admitted=0;
   FILE *fp;
   fp = fopen(paths_filename,"w");
   long double b_eq;
   for(int i=0;i<con_num;i++){
      cout<<endl<<"checking connection number = "<<i<<endl;
      if(((int)o_or_p )== 0){
           b_eq = computeMin(connections[i][4],connections[i][3]+(0.25*(connections[i][4]-connections[i][2])));
      }
      else{
          b_eq = connections[i][4];
      }
      bool is_sp;
      int sp_id = -1;
      src = connections[i][0];
      dest = connections[i][1];
      is_sp = conditionToMet(fir_spath,src,dest,net_top,b_eq);
      if(is_sp){
        sp_id = 1;
      }
      else{
         is_sp = conditionToMet(sec_spath,src,dest,net_top,b_eq);
         if(is_sp){
             sp_id = 2;
         }
      }

      if(sp_id!=-1){
         cout<<endl<<"connection number = "<<i<<"admitted"<<endl;
         conn_admitted++;
         if(sp_id==1){
             updateForwardingTable(forwardingtable,fir_spath,nodes,label,interface,src,dest,i);
             fprintf(output,"]   %d\n\n",(int)first_second[src][dest][0]);
         }
         else{
            updateForwardingTable(forwardingtable,sec_spath,nodes,label,interface,src,dest,i);
            fprintf(output,"]   %d\n\n",(int)first_second[src][dest][1]);
         }
      }
      else{
         cout<<endl<<"connection number = "<<i<<" not admitted"<<endl;
         continue;
      }
   }
   cout<<endl<<"number of connections admitted = "<<conn_admitted<<" out of "<<con_num<<endl;
   fprintf(fp,"%d %d",con_num,conn_admitted);
   fclose(fp);
   fclose(output);
}

void printForwardingTable(int forwardingtable[][maximum_conn][4],char filename[],int total_connections,int nodes){

 FILE *fp;
 fp = fopen(filename,"w");
 for(int i=0;i<nodes;i++){

    fprintf(fp,"\nfor node number = %d\n\n",i);
    for(int j=0;j<total_connections;j++){
        if(forwardingtable[i][j][0]==-1){
           break;
        }
        fprintf(fp,"%d\t%d\t%d\t%d\n",forwardingtable[i][j][0],forwardingtable[i][j][1],forwardingtable[i][j][2],forwardingtable[i][j][3]);
    }
 }
  fclose(fp);
}


int main(int argc,char** argv)
{
    long double network_topology[maximum_nodes][maximum_nodes][2];
    char input_topology_filename[100],connections_filename[100],routingtable_filename[100],forwardingtable_filename[100],paths_filename[100],type_of_input[100],opt_or_pess;
    int connections[maximum_conn][5],conn_count;
    strcpy(input_topology_filename, argv[2]);
	  strcpy(connections_filename, argv[4]);
	  strcpy(routingtable_filename, argv[6]);
	  strcpy(forwardingtable_filename, argv[8]);
	  strcpy(paths_filename, argv[10]);
	  strcpy(type_of_input, argv[12]);
    opt_or_pess = atoi(argv[14]);
    int number_of_nodes,number_of_edges;
    memset(network_topology,0,sizeof(network_topology));
    readInputTopology(input_topology_filename,network_topology,&number_of_nodes,&number_of_edges);
    long double cost_parent[maximum_nodes][maximum_nodes][2],cost_parent_new[maximum_nodes][maximum_nodes][2],first_second_costs[maximum_nodes][maximum_nodes][2]; // cost_parent[source][dest][0] is linkcost, cost_parent[source][dest][1] is parent;
    memset(first_second_costs,-1,sizeof(first_second_costs));
    assignCostParent(cost_parent,network_topology,number_of_nodes,type_of_input);
    allPairsShortestPath(cost_parent, number_of_nodes);
    long double first_shortest_path[maximum_nodes][maximum_nodes][maximum_nodes], sec_shortest_path[maximum_nodes][maximum_nodes][maximum_nodes];
    memset(first_shortest_path,-1,sizeof(first_shortest_path));
    memset(sec_shortest_path,-1,sizeof(sec_shortest_path));
    storeShortestPath(cost_parent,number_of_nodes,first_shortest_path,first_second_costs);
    assignCostParent(cost_parent_new,network_topology,number_of_nodes,type_of_input);
    findSecondShortestPath(network_topology,cost_parent_new,number_of_nodes,sec_shortest_path,first_shortest_path,type_of_input,first_second_costs);
    storeRoutingInfoInFile(routingtable_filename,first_shortest_path,sec_shortest_path,number_of_nodes,type_of_input,network_topology);
    readConnectionsFile(connections_filename,connections,&conn_count);
    int label[maximum_nodes][maximum_nodes];
    memset(label,-1,sizeof(label));
    assignLabels(network_topology,number_of_edges,label,number_of_nodes);
    int interface[maximum_nodes][maximum_nodes];
    memset(interface,-1,sizeof(interface));
    assignInterfaces(label,number_of_nodes,interface);
    int forwardingtable[maximum_nodes][maximum_conn][4];
    memset(forwardingtable,-1,sizeof(forwardingtable));
    checkConnections(forwardingtable,label,interface,number_of_nodes,opt_or_pess,forwardingtable_filename,paths_filename,connections,conn_count,first_shortest_path,sec_shortest_path,network_topology,first_second_costs,paths_filename);
    printForwardingTable(forwardingtable,forwardingtable_filename,conn_count,number_of_nodes);
    return 0;
}
