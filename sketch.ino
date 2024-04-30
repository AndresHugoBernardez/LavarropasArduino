
// Lavaropas automático (sin centrifugado): 
//
// Hecho por Andrés Hugo Bernárdez
// 2024
//
//Objetivo: Llevar lavaropas semi-automático para gente de bajos recursos.
// El lavaropas tendrá:
//     * electroválvula común: Relé del superior
//     * Motor para lavar: Relé del medio
//     * bomba de desagote común Relé inferior

//    Los 3 representados con leds de color violeta para saber cuándo encienden


// SENSORES
// Representados con botones rojos:
// Boton rojo arriba: Sensor de nivel alto (listo para lavar/enjuagar)
// Boton rojo Abajo: Sensor de nivel de agua desagotada
// LA CONFIGURACIÓN ELECTRICA DE LOS SENSORES AQUÍ SOLO ES DE MUESTRA Y
// DEBE TENERSE EN CUENTA LA CONFIGURACIÓN DE LOS DISPOSITIVOS MEDIDORES 
// DE NIVEL REALES


// Además tendrá Programa de lavado 
//   Led rojo: Lavado largo
//   Led verde: Lavado común
//   Led azul: Enjuague.
//   BOTÓN VERDE:Botón para selección de programa
//   BOTÓN AMARILLO: cuando se abre la tapa



//Pines seleccionados:

const int ReleBomba=7;
const int ReleElectroValvula=6;
const int ReleMotorLavar=5;


const int LedLargo=13;
const int LedComun=12;
const int LedEnjuague=11;

const int BotonPrograma=4;
const int BotonPausa=8;

const int NivelMaximo=10;
const int NivelDesagotado=9;


//Tiempos Seleccionados
// en minutos
// NOTA los tiempos se pusieron en 1 minuto cada uno para poder simularlos.
//********ATENCIÓN: CORRIJA SU TIEMPO PARA SU USO************************************************************<<<<<<<<<<

const int TiempoLavado=1;
const int TiempoEnjuague=1;



//  Programas
const int NINGUNO=0;
const int LARGO=1;
const int COMUN=2;
const int ENJUAGUE=3;



//    Estados del lavarropas

const int NADA=0;
const int MOTORON=1;
const int LLENANDO=2;
const int VACIANDO=3;


//    Variables Globales:

int ProgramaActual=LARGO;
int BotonPresionado=0;
int Estado=NADA;



//Declaración previa de estructura de funciones de uso común (protocolo de C general)
void Pausa(void);
void loop(void);


///**************************************
//Configuración de variables
//
void setup() {
  

  pinMode(ReleBomba,          OUTPUT);
  pinMode(ReleElectroValvula, OUTPUT);
  pinMode(ReleMotorLavar,     OUTPUT);

  pinMode(LedLargo,       OUTPUT);
  pinMode(LedComun,       OUTPUT);
  pinMode(LedEnjuague,    OUTPUT);

  pinMode(BotonPrograma,  INPUT);
  pinMode(BotonPausa,     INPUT);

  // la configuración del nivel es solo un prototipo, el medidor de nivel
  // debe corresponderse con el real.  
  pinMode(NivelMaximo     , INPUT);
  pinMode(NivelDesagotado , INPUT);

}



///**************************************
// Manejo de retardos
//




///EsperarMinuto()
// Espera 1 minuto y mientras tanto revisa si no se ha tocado el botón de programa
void EsperarMinuto()
{
  int i,boton;

  for(i=0;i<600;i++)
  {

    delay(100);

    // Si se presiona el botón de pausa, se detiene momentaneamente hasta soltarlo
    if(digitalRead(BotonPausa)==1)Pausa();

    // Si en el transcurso del minuto se presiona el botón de programa (el verde)
    // Se detiene todo automaticamente y se cambia el programa. 
    if(digitalRead(BotonPrograma)==1) {
      BotonPresionado=1;
      loop();
      i=600;
    };
  }
}

///EsperarSegundos(int segundos)
// Espera 1 minuto y mientras tanto revisa si no se ha tocado el botón de programa
void EsperarSegundos(int segundos)
{
  int i,boton;

  for(i=0;i<(segundos*10);i++)
  {

    delay(100);

    // Si se presiona el botón de pausa, se detiene momentaneamente hasta soltarlo
    if(digitalRead(BotonPausa)==1)Pausa();

    // Si en el transcurso  se presiona el botón de programa (el verde)
    // Se detiene todo automaticamente y se cambia el programa. 
    if(digitalRead(BotonPrograma)==1) {
      BotonPresionado=1;
      loop();
      i=segundos+1;
      
    };
  }
}


///EsperarSegundos(int segundos)
// Espera 1 minuto y mientras tanto revisa si no se ha tocado el botón de programa
void EsperarDecimaDeSegundo(int Decima)
{
  int i,boton;

  for(i=0;i<Decima;i++)
  {

    delay(100);


    // Si se presiona el botón de pausa, se detiene momentaneamente hasta soltarlo
    if(digitalRead(BotonPausa)==1)Pausa();


    // Si en el transcurso  se presiona el botón de programa (el verde)
    // Se detiene todo automaticamente y se cambia el programa. 
    if(digitalRead(BotonPrograma)==1) {
      BotonPresionado=1;
      loop();
      i=Decima+1;
      
    };
  }
}


///**************************************
// RELES:
//


///ApagarReles()
// apagar todos los reles
  int ApagarReles() 
 {
  digitalWrite(ReleBomba, LOW);
  digitalWrite(ReleElectroValvula, LOW);
  digitalWrite(ReleMotorLavar, LOW);
 }

///MotorON:
// Enciende el motor de la lavadora durante el tiempo indicado en minutos
void MotorON(int tiempo)
{
  
  int i;

  // apagar todos los reles
  ApagarReles();

  //esperar 2 segundos
  EsperarSegundos(2);

  // Encender rele del motor lavadora
  Estado=MOTORON;
  digitalWrite(ReleMotorLavar, HIGH);
  

  //Esperar hasta que se cumpla el tiempo
  for(i=0;i<tiempo && BotonPresionado==0;i++)
  {
    EsperarMinuto();
  }

   Estado=NADA;
   digitalWrite(ReleMotorLavar, LOW);


}

///Llenar:
// Llena el lavarropas mediante la electroválvula hasta que el sensor de llenado (botón rojo superior)
// indique que ya se llenó
void Llenar()
{
  
  int i,nivel;

  // apagar todos los reles
  ApagarReles();

  //esperar 2 segundos
  EsperarSegundos(2);

  // Encender rele de la electrovalvula
  Estado=LLENANDO;
  digitalWrite(ReleElectroValvula, HIGH);
  
  //Esperar hasta el nivel máximo
  nivel=0;
  do
  {
    EsperarDecimaDeSegundo(1);
    nivel=digitalRead(NivelMaximo);
  }
  while(nivel!=1 && BotonPresionado==0);

  //apagando electrovalvula
  Estado=NADA;
   digitalWrite(ReleElectroValvula, LOW);


}

///Vacía:
// Llena el lavarropas mediante la bomba hasta que el sensor de vaciado (botón rojo inferior)
// indique que ya se vació
void Vaciar()
{
  
  int i,nivel;

  // apagar todos los reles
  ApagarReles();

  //esperar 2 segundos
  EsperarSegundos(2);

  // Encender rele de la bomba
  Estado=VACIANDO;
  digitalWrite(ReleBomba, HIGH);
  

  // Esperar hasta que se haya desagotado
  nivel=0;
  do
  {
    EsperarDecimaDeSegundo(1);
    nivel=digitalRead(NivelDesagotado);
  }
  while(nivel!=1 && BotonPresionado==0);

  //Apagar Bomba
  Estado=NADA;
  digitalWrite(ReleBomba, LOW);


}


///**************************************
// PROGRAMAS
//


///LedsMode()
// Indica en que modo de lavado se encuentra
void LedsMode(int Modo){

  switch(Modo)
  {
    case LARGO:   digitalWrite(LedLargo,    HIGH);
                  digitalWrite(LedComun,    LOW);
                  digitalWrite(LedEnjuague, LOW);
                  break;
    case COMUN:   digitalWrite(LedLargo,    LOW);
                  digitalWrite(LedComun,    HIGH);
                  digitalWrite(LedEnjuague, LOW);
                  break;
    case ENJUAGUE:digitalWrite(LedLargo,    LOW);
                  digitalWrite(LedComun,    LOW);
                  digitalWrite(LedEnjuague, HIGH);
                  break;
    case NINGUNO: digitalWrite(LedLargo,    LOW);
                  digitalWrite(LedComun,    LOW);
                  digitalWrite(LedEnjuague, LOW);
                  break;
    default:break;
  }




}


///CambiarPrograma()
//Cambia el programa al siguiente en el orden LARGO-COMUN-ENJUAGUE-NINGUNO
void CambiarPrograma(){

  if(ProgramaActual==LARGO)
  {
    ProgramaActual=COMUN;
    LedsMode(COMUN);
    delay(500);
  }
  else if(ProgramaActual==COMUN)
  {
    ProgramaActual=ENJUAGUE;
    LedsMode(ENJUAGUE);
    delay(500);
  }
  else if(ProgramaActual==ENJUAGUE)
  {
    ProgramaActual=NINGUNO;
    LedsMode(NINGUNO);
    delay(500);
  }
  else if(ProgramaActual==NINGUNO)
  {
    ProgramaActual=LARGO;
    LedsMode(LARGO);
    delay(500);
  }
}



///**************************************
//  PAUSA:
//

///Pausa()
// Se espera hasta que se deje de presionar el boton de pausa
void Pausa(){
  int sigue=1,cambio=0;

  // apagar todos los reles
  ApagarReles();


  while (sigue==1){

    //verificar que siga presionado el botón de pausa.
    sigue=digitalRead(BotonPausa);

    // Si mientras está pausado se presiona el boton de programa entonces se cambia el programa
    if(digitalRead(BotonPrograma)==1){
      CambiarPrograma();
      BotonPresionado=0;
      cambio=1; 
    }

    //Esperar 0,1 segundos
    delay(100);

  }


  //si mientras estaba pausado se presionó el botón programa se iniciará en ese programa.
  if(cambio==1)loop();


  //Esperar 1 segundo antes de continuar
  delay(1000);


  //volver al estado anterior
  switch(Estado){
    
    case MOTORON:   digitalWrite(ReleMotorLavar, HIGH);
                    break;
    case LLENANDO:  digitalWrite(ReleElectroValvula, HIGH);
                    break;
    case VACIANDO:  digitalWrite(ReleBomba, HIGH);
                    break;
    default:break;
  }


}



//****************************************************
//****************************************************
//***************>>>BUCLE PRINCIPAL<<<****************
//****************************************************
//****************************************************




void loop() {


  

  if(BotonPresionado==1){
    ApagarReles();
    CambiarPrograma();
    BotonPresionado=0;
    EsperarSegundos(2); 
  }

  switch(ProgramaActual){

    case LARGO:         LedsMode(LARGO);
                        if(BotonPresionado==0)  Llenar();
                        if(BotonPresionado==0)  MotorON(TiempoLavado);
                        if(BotonPresionado==0)  Vaciar();
                        if(BotonPresionado==0)  ProgramaActual=COMUN;
                        if(BotonPresionado==0)  LedsMode(COMUN);
                        if(BotonPresionado==0)  EsperarSegundos(5); 
    case COMUN:       
                        if(BotonPresionado==0)  Llenar();
                        if(BotonPresionado==0)  MotorON(TiempoLavado);
                        if(BotonPresionado==0)  Vaciar();
                        if(BotonPresionado==0)  ProgramaActual=ENJUAGUE;
                        if(BotonPresionado==0)  LedsMode(ENJUAGUE);
                        if(BotonPresionado==0)  EsperarSegundos(5); 
    case ENJUAGUE:       
                        if(BotonPresionado==0)  Llenar();
                        if(BotonPresionado==0)  MotorON(TiempoEnjuague);
                        if(BotonPresionado==0)  Vaciar();
                        if(BotonPresionado==0)  ProgramaActual=NINGUNO;
                        if(BotonPresionado==0)  LedsMode(NINGUNO);
                        if(BotonPresionado==0)  EsperarSegundos(5);  
    case NINGUNO:       
  
                        if(BotonPresionado==0)  EsperarSegundos(5);                       


  }

}



