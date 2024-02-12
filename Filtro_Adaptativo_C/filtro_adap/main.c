#include <stdio.h>
#include <fcntl.h>
#include <io.h>


#define NSAMPLES 4	// Tamanho da m�dia

int main()
{
   FILE *in_file, *out_file;
   int i, n, n_amost;
  
   short entrada, saida;
   float sample[NSAMPLES] = {0x0};
   float w[NSAMPLES] = {0x0};

   float y=0;
   float erro=0;
   float d=0;
   float u=0.000000000001;

   //Carregando os coeficientes do filtro m�dia m�vel
   
   float planta[NSAMPLES]={
   				#include "coefs_mm_4.dat"
   };
  
 
   /* abre os arquivos de entrada e saida */
  if ((in_file = fopen("./ruido_branco.pcm","rb"))==NULL)
  {
    printf("\nErro: Nao abriu o arquivo de entrada\n");
    return 0;
  }
  
  if ((out_file = fopen("./sai_ruido_branco.pcm","wb"))==NULL)
  {
    printf("\nErro: Nao abriu o arquivo de saida\n");
    return 0;
  }


   // zera vetor de amostras
   for (i=0; i<NSAMPLES; i++)
        {
        sample[i]=0;
        }

   // execu��o do filtro
 do {
        
	   //zera sa�da do filtro
        d=0;
        y=0;
        erro=0;

        //l� dado do arquivo
        n_amost = fread(&entrada,sizeof(short),1,in_file);
		sample[0] = entrada;

        //Convolu��o e acumula��o
        for (n=0; n<NSAMPLES; n++){
            d += planta[n]*sample[n];
        }

        //w0, w1, w2, w3
        for (n=0; n<NSAMPLES; n++){
            y += sample[n]*w[n];
        }

        erro = d - y;

        for (n=0; n<NSAMPLES; n++){
            w[n] = w[n] + u * erro * sample[n];
        }        

        //desloca amostra
        for (n=NSAMPLES-1; n>0; n--)
        {
            sample[n]=sample[n-1];
        }

		saida = (short) erro;

        //escreve no arquivo de saida
        fwrite(&saida,sizeof(short),1,out_file);

 } while (n_amost);

    //printf("%.6f", w[0]);
    //fecha os arquivos de entrada de saida
    fclose(out_file);
    fclose(in_file);
   return 0;
}
