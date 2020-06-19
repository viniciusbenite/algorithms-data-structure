////
// Bernardo Rodrigues
// n_Mec = 88835
// ...
//
// AED, 2018/2019
//
// solution of the traveling salesman problem
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "cities.h"
#include "../P01/elapsed_time.h"


//
// record best solutions
//

/*Struct criado para guardar dados*/
/*guarda a distancia minima,distancia maxima,o melhor caminho e o pior caminho*/
struct guardar_dados {
   int  min_distance;
   int  max_distance;
   int  max_tour_str[max_n_cities+1];
   int  min_tour_str[max_n_cities+1];
   double tour_time;
 };
static struct guardar_dados dados[max_n_cities+1];
/*Array de structs é criado para guardar as informações de n cidades
ex:    dados[3] guarda informações de um percurso com 3 cidades
/      dados[4] informações de um percurso de 4 cidades
No final dos calculos dos percursos é usado o struct para 
ver os dados*/
int p=0;    /*Esta variavel é usada para criar 2 vetores com o tamanho do maior percurso*/


static int min_length,max_length,VISITED_ALL,mask;
static int min_tour[max_n_cities + 1],max_tour[max_n_cities + 1],dp[1<<max_n_cities][max_n_cities];
static long n_tours;
//static int debug = 0;     /*Imprime todos os caminhos e a distancia   AUMENTA O TEMPO DO PROGRAMA*/


int  tsp(int mask,int pos, int n){

	n_tours++;

	if(mask==VISITED_ALL){
		return cities[pos].distance[0];
	}

	if(dp[mask][pos]!=-1){
		return dp[mask][pos];
	}


	int min_ans = 999999;

	for(int city=0;city<n;city++){

		if((mask &(1<<city))==0){
			int newAns = cities[pos].distance[city] + tsp( mask|(1<<city), city , n);
			if (min_ans>newAns){
				min_ans = newAns;
			}
		}

	}
	dp[mask][pos]=min_ans;
	return min_ans;
}







int  tsp_max(int mask,int pos, int n){

	if(mask==VISITED_ALL){
		return cities[pos].distance[0];
	}

	if(dp[mask][pos]!=-1){
	   return dp[mask][pos];
	}


	int max = 0;

	for(int city=0;city<n;city++){

		//printf("%d\n", (1<<city));

		if((mask &(1<<city))==0){

			int newAns = cities[pos].distance[city] + tsp_max( mask|(1<<city), city , n);
			//printf("newAns: %d\n",newAns );
			//printf("%d\n", newAns);
			if (max<newAns){
				max = newAns;
			}
		}

	}

	dp[mask][pos]=max;
	return max;
}





//
// main program
//

int main(int argc,char **argv)
{
  int n_mec,special,n,i,a[max_n_cities];
  char file_name[32];
  double dt1;

  n_mec = 0; // CHANGE THIS!
  special = 0;
  init_cities_data(n_mec,special);
  printf("data for init_cities_data(%d,%d)\n",n_mec,special);
  fflush(stdout);


#if 0
  print_distances();
#endif
  for(n = 3;n <= (max_n_cities-13);n++)  /*AUMENTAR OU DIMINUIR--MAXIMO 18 CIDADES*/
  {




    dt1 = -1.0;
    if(n <= 16)
    {
      (void)elapsed_time();
      for(i = 0;i < n;i++)
        a[i] = i;
      min_length = 1000000000;
      max_length = 0;
      n_tours = 0l;

      //-----------------------------------//
      VISITED_ALL = (1<<n) -1;
      mask = 1;

	  for(int i=0;i<(1<<max_n_cities);i++){
  	  	for(int j=0;j<max_n_cities;j++){
    		dp[i][j] = -1;
    	}
  	   }
      min_length = tsp(mask,0, n); // no need to change the starting city, as we are making a tour


      for(int i=0;i<(1<<max_n_cities);i++){
  		for(int j=0;j<max_n_cities;j++){
    		dp[i][j] = -1;
    	}
  	  }
      max_length = tsp_max(mask,0, n);
      //-----------------------------------//


      dt1 = elapsed_time();
      dados[n].tour_time = dt1;         /*Regista o tempo de cada tour no struct dados*/
      printf("tsp_v1() finished in %8.3fs (%ld tours generated)\n",dt1,n_tours);
      printf("  min %5d [",min_length);
      for(i = 0;i < n;i++)
        printf("%2d%s",min_tour[i],(i == n - 1) ? "]\n" : ",");

      memcpy(dados[n].min_tour_str, min_tour, sizeof(min_tour));    /*Regista o menor tour*/
      dados[n].min_distance = min_length;   /*Update da distancia minima para um tour com n cidades*/

      printf("  max %5d [",max_length);
      for(i = 0;i < n;i++)
        printf("%2d%s",max_tour[i],(i == n - 1) ? "]\n" : ",");

      memcpy(dados[n].max_tour_str, max_tour, sizeof(max_tour));    /*Regista o maior tour*/
      dados[n].max_distance = max_length;   /*Update da distancia maxima para um tour com n cidades*/

      fflush(stdout);
      if(argc == 2 && strcmp(argv[1],"-f") == 0)
      {
        min_tour[n] = -1;
        sprintf(file_name,"min_%02d.svg",n);
        make_map(file_name,min_tour);
        max_tour[n] = -1;
        sprintf(file_name,"max_%02d.svg",n);
        make_map(file_name,max_tour);
      }
    }
  }

  /*No final do programa é impresso uma tabela com as informações dos percursos*/
  printf("----------------------------------------------------------------------------------------------------------------------");
  printf("\n%s%12s%12s%40s%12s%40s\n" , "n" , "minLength" , "minPath" , "maxLength" , "maxPath" , "tempo");

  int formatar = 40;/*Esta variavel é usada para formatar os printf para facilitar a leitura das informações da tabela*/

  for (n = 3; n <= max_n_cities; n++)
  {
    if (dados[n].min_distance == 0){
      p = n;    /*Esta variavel é usada mais à frente no programa para definir o tamanho de 2 vetores*/
      break;    /*Se dados[n].min_distance == 0 significa que não feito um percurso para n cidades*/
    }

    printf("%-9d", n);      /*Imprime o numero de cidades de um percurso*/

    printf("%-9d[",  dados[n].min_distance);      /*Imprime a distancia minima*/

    for (i = 0; i < n; i++) {
      printf("%d%s",dados[n].min_tour_str[i],(i == n - 1) ? "]" : ",");     /*Imprime o melhor caminho*/
    }
    printf("%*d%-5s[", formatar , dados[n].max_distance , "");      /*Imprime a distancia maxima*/

    for (i = 0; i < n; i++) {
      printf("%d%s",dados[n].max_tour_str[i],(i == n - 1) ? "]" : ",");     /*Imprime o pior caminho*/
    }

    printf("%*f%s\n", formatar , dados[n].tour_time , "s");     /*Imprime o tempo*/

    if (n<10)
      formatar = formatar -2;  /*No proximo printf a distancia do minPath para o maxLength,que depende da variavel formatar, diminui*/
    else
      formatar = formatar - 3;  /*Quando n>10 o pior caminho ocupa mais um espaço para escrever*/
  }

  /*LEGENDA-> print das cidades e o respetivo numero
  para facilitar a leitura dos minPath e maxPath*/
  printf("%s\n", "LEGENDA DAS CIDADES:");
  for (i = 0 ; i<=max_n_cities ; i++){
    if (a[i]==0 && i!= 0) 
        break;
    printf("%s-", cities[a[i]].name);
    printf("%d\n", a[i]);
    }

    printf("----------------------------------------------------------------------------------------------------------------------\n");



    /*Criação dos mapas, para estes poderem ser desenhados, foram criados*/
    /*2 vetores com o tamanho do nº de cidades do maior tour, visto que o programa demora*/
    /*demasiado tempo quando o é usado o max_n_cities*/
    /*e porque as posições finais do min_tour e max_tour*/
    int min_tour_map[p + 1] , max_tour_map[p + 1];

    for (i = 0 ; i<=(max_n_cities-6) ; i++){        //FOR ACABA NO NUMERO DE CIDADES CALCULDAS - 1          (max_n_cities - 1)
        min_tour_map[i] = min_tour[i];
        max_tour_map[i] = max_tour[i];
    }
   min_tour_map[p] = -1;
   max_tour_map[p] = -1;

   make_map("pt_best_tour.svg",min_tour_map);
   make_map("pt_worst_tour.svg",max_tour_map);

  return 0;
}
