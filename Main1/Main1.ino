teste ss

/*esqueleto de codigo para implementar*/
/*
  --------------------------------------inicizalizacao---------------------------
  funcao de inicializar os sensores/ponte H/ motores/ encoders
*/
/*______________________________________________FUNCOES DE ALTO NIVEL !!_________________________________________________*/
/*
 funcao que recebe duas coordenadas (X,Y) atuais e retorna o local do robo numa matriz de 9 (qual casa)
 |20|21|22|
 |10|11|12|
 |00|01|02|
 ///////////////////// NOTE QUE AS AREAS NAO VAO SER QUADRADAS !! VOA DEPENDER DO TAMANHO DA ARENA E DOS QUARTOS
*/
/*_______________________________________________________________________________________________________________________*/  
/*
  funcao que seria chamada quando o robo estiver na porta de cada quarto que procura a vela ! -------- se achar, sera chamada a funcao de baixo - apagar a vela 
*/
/*_______________________________________________________________________________________________________________________*/
/*
  se a funcao de cima achar a vela essa funcao sera chamada e ela realizaria o processo de apagar a vela
*/
/*_______________________________________________________________________________________________________________________*/
/*
  sair do quarto e voltar para a orientacao certa para continur a trajetoria do robo caso a funcao de procurar a vela nao achar a vela
*/
/*_______________________________________________________________________________________________________________________*/
/*
  realiza a volta pra base sem depender do local que o robo atualmente esta ! usando a matriz da primeira funcao
*/
/*_______________________________________________________________________________________________________________________*/
/*
  comparacao entre as coordenadas X e Y atuais e a quantiadade de vao passados
*/
/*_______________________________________________________________________________________________________________________*/
/*
 caso a funcao de cima retorna que o valor das coordenadas eh inconveniente com a quantidade de vaos passados
 esta funcao faria o robo ir ao checkpoint para recomecar a parte na qual  ele se perdeu
*/
/*_______________________________________________________________________________________________________________________*/
/*
 ---------------------------------------FUNCAO CORACAO DO CODIGO !--------------------------
 ----DIRECIONARIA O ANDAMENTO DO CODIGO PARA AS OUTRAS FUNCOES USANDO VARIAVEL GLOBAR DO ESTADO ATUAL DO ROBO (o nome dessa variavel provavelmente sera busy)! 
 --------------- RECOMENDO DEIXAR A IMPLEMENTACAO DESSA FUNCAO PARA O ABDU/MINIBAU
*/
/*______________________________________-----------------------------------______________________________________________*/
/*--------------------------------------___________________________________----------------------------------------------*/

/*______________________________________FUNCOES DE BAIXO NIVEL !!!!!!!!!!!!______________________________________________*/
/*
  leitura de US instantanea + identificacao de vao somando no contador global a quantidade de vao passados para cada (esq/ dir)
*/
/*_______________________________________________________________________________________________________________________*/
/*
  funcao que recebe a distancia como parametro e anda essa distancia pedida. (poderia ser arrompida pela leitura do vao no US)
*/
/*_______________________________________________________________________________________________________________________*/
/*
  funcoes de girar (podem ser varias para cada um angulo e direcao, ou uma que recebe angulo e direcao como parametro e faz para qualquer angulo "funcao inteligente")
  ---para escolher qual o tipo dessa funcao, recomendo o segundo pq poderia ser reutilizado e seria bem mais inteligente 
  MAS*** as vezes esse tipo de funcao eh mais arriscado do que implementar funcao para cada caso! entao se n der muito certo o segundo tipo, pode ser o primeiro
*/
/*_______________________________________________________________________________________________________________________*/
/*
 filtros para os sensores que necessitam
*/

void setup()
{
  /*
      setttar as parada todim
  -----------------------------------------------------------
    chamar a funcao de inicializar os bagul
    ;
    e anda certa distancia ate o vao atravessando a escada
    depois de andar, somar o valor da distancia entre o ponto de inicio ate o final da escada (projetado no x ja)
  */
}

void loop()
{
}
