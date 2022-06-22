#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Wire.h>

#define PLANTA 11
#define AGUA 12
#define FUEGO 13
#define DRAGON 14
#define HADA 15
#define ACERO 16

#define CURSORLCDINICIO lcd.setCursor(0, 0);
#define CURSORLCD2FILA lcd.setCursor(0, 1);
#define CLEARLCD lcd.clear();
#define OBTENERTECLA keypad.getKey();
#define ESPERARTECLA keypad.waitForKey();

volatile char msj[20];
char CMD;
bool hayJugadorDerrotado;

class Pokemon
   {  
      private:
          String nombre;  
    	  int tipo;
    	  int vida;
          int ataqueNeutro;
    	  int ataqueElemental;
  
      public:
  Pokemon(String name, int type, int life, int neutro, int elemental)
             {  nombre =name;  
    	  		tipo=type;
    	  		vida=life;
          		ataqueNeutro=neutro;
    	  		ataqueElemental=elemental; 	
             }  
  String getNombre()
  {return nombre;}
  
  int getSalud()
  {return vida;}
  
  int getAtaqueNeutro()
  {return ataqueNeutro;}
  
   int getAtaqueElemental()
  {return ataqueElemental;}
  
  
  String getTipo()
  {
    String retorno="";
    switch(tipo)
  {
  	case PLANTA: retorno="PLANTA";
      			 break;
	case AGUA: retorno="AGUA";
      			break;
	case FUEGO: retorno="FUEGO";
      			break;
	case DRAGON: retorno="DRAGON";
      			break;
	case HADA: retorno="HADA";
      			break;
	case ACERO: retorno="ACERO";
      			break;
  }
    return retorno;
  }
  
   };

Pokemon nulo("", 0, 0, 0, 0);
 
Pokemon serperior("SERPERIOR", PLANTA, 115, 10, 24);
Pokemon blastoise("BLASTOISE", AGUA, 105, 12, 24);
Pokemon arcanine("ARCANINE", FUEGO, 80, 18, 30);
Pokemon goodra("GOODRA", DRAGON, 95, 14, 26);
Pokemon sylveon("SYLVEON", HADA, 110, 12, 24);
Pokemon melmetal("MELMETAL", ACERO, 100, 10, 28);

Pokemon POKEOPCIONES[]={serperior, blastoise, arcanine, goodra, sylveon, melmetal};

LiquidCrystal lcd(A1, A0, A2, A3, 2, 3);

const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {11, 10, 9, 8}; 
byte colPins[COLS] = {7, 6, 5, 4}; 

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
char key;

Pokemon EQUIPO[]={nulo,nulo,nulo};
int cantEquipo = 0;
bool INICIO, BATALLA, RIVALELIGIO, TEAMREADY;
volatile int danio=0;
volatile int danioRival=0;
int POKEACTIVO=0;//SIEMPRE EL PRIMERO POR DEFECTO
byte CODE;

void setup(){
  
   lcd.begin(16, 2);
   CURSORLCDINICIO
   INICIO=true;
   TEAMREADY=false;
   BATALLA=false;
   RIVALELIGIO=false;
   Wire.begin(0x20);
   Wire.onReceive(recibirDatos);
   Wire.onRequest(enviarDatos);
 }
  
void loop(){
  if(INICIO)
  {
   if(!TEAMREADY)
    {
      msjSeleccion();
      key = ESPERARTECLA
      if (key=='#')
      {
        elegirEquipo();
       }
    }else if(TEAMREADY){
     
     if(RIVALELIGIO)
      {
        INICIO=false;
      	BATALLA=true;
       hayJugadorDerrotado=false;
        CLEARLCD
      }else{
         msjEsperarRival();
      }
    }
  } 
  if(BATALLA)
  {
    if(!hayJugadorDerrotado)
    {
      danio=0;
      danioRival=0;
      RIVALELIGIO=false;
     correrTurno();}
    else if(hayJugadorDerrotado)
    {CLEARLCD
    CURSORLCDINICIO
    lcd.print("PROCESAR FINAL");
    ESPERARTECLA
    }
  }
  
  
}

void correrTurno()
{
  bool accion=false;
  while(!accion)
  {
    msjMenuBatalla();
    key=OBTENERTECLA
      if (key=='1')
      {
        accion=atacar();
      }else if (key=='2')
      {
        accion=cambiarPoke;
      } else if (key=='3')
      {
        verInfo();
      }
  }
  CLEARLCD
  while(!RIVALELIGIO)
  {
   msjEsperarRival();
  }
  CURSORLCDINICIO
  lcd.print(danio);
  CURSORLCD2FILA
  lcd.print(danioRival);
  ESPERARTECLA
}

bool atacar()
{
    bool volver = false;
   bool accion = false;
   while(!volver && !accion)
   {
		CLEARLCD
    	CURSORLCDINICIO
		lcd.print("1-NEUTRO");
    	CURSORLCD2FILA
    	lcd.print("2-ELEMENTAL");
    	key = ESPERARTECLA
     	if (key == '1')
        {	do{CLEARLCD
    		CURSORLCDINICIO
			lcd.print("CONF NEUTRO #");
            key = ESPERARTECLA
              if(key == '#')
            {danio=EQUIPO[POKEACTIVO].getAtaqueNeutro();
            accion=true;}}
         while(key!='#' && key!= '*');
    	}else if (key == '2')
        {
          do{CLEARLCD
    		CURSORLCDINICIO
			lcd.print("CONF ELEM #");
            key = ESPERARTECLA
              if(key = '#')
            {danio=EQUIPO[POKEACTIVO].getAtaqueElemental();
             accion=true;}}
          while(key!='#' && key!= '*');
    	}else if (key=='*')
     	{volver=true;}
   }
  CLEARLCD
    return accion;
}

bool cambiarPoke()
{
   bool volver = false;
   bool accion = false;
   int num;
   int pvalido[2];
   while(!volver && !accion)
   {
     int p=0;
     int mostre=0;
		CLEARLCD
    	CURSORLCDINICIO
        while(mostre+1<cantEquipo)
        {
          if (p!=POKEACTIVO&&EQUIPO[p].getSalud()!=0)
         {
          lcd.print(p+1);
          lcd.print(EQUIPO[p].getNombre());
    	  lcd.print(":");
    	  lcd.print(EQUIPO[p].getSalud());
    	  lcd.print("HP");
  		  CURSORLCD2FILA
           pvalido[mostre]=p;
          mostre++;  
         }
          p++;
          if(p==3)
          {p=0;}
        }
    	key=ESPERARTECLA
        num = key-'0';
        if((num-1)==pvalido[0]||(num-1)==pvalido[1])
        {  
          do{CLEARLCD
    	  CURSORLCDINICIO
          lcd.print(EQUIPO[num-1].getNombre());
          CURSORLCD2FILA
          lcd.print("CONF #");
          key=ESPERARTECLA
            if(key=='#')
          	{
             POKEACTIVO=num-1;
             danio=1; //flag que es cambio
             accion=true;
            }
           }while(key!='#' && key!= '*');
        }else if(key =='*' )
        {volver=true;}
   }
  CLEARLCD
    return accion;
}

void verInfo()
{
	CLEARLCD
    CURSORLCDINICIO
  	lcd.print(EQUIPO[POKEACTIVO].getNombre());
    lcd.print(":");
    lcd.print(EQUIPO[POKEACTIVO].getSalud());
    lcd.print("HP");
  	CURSORLCD2FILA
    short i=0;
    while(msj[i] != '-')
    {lcd.print(msj[i]);
     i++;}
  do{key=ESPERARTECLA;}
  	while(key!='*');
  CLEARLCD
}


void msjSeleccion()
{
  CURSORLCDINICIO
  lcd.print("Selecciona a tu");
  CURSORLCD2FILA
  lcd.print("equipo: #");
}

void msjEsperarRival()
{
  CURSORLCDINICIO
  lcd.print("Esperando al");
  CURSORLCD2FILA
  lcd.print("rival...");
}

void msjMenuBatalla()
{
  CURSORLCDINICIO
  lcd.print("1.Atk 2.Cambiar");
  CURSORLCD2FILA
  lcd.print("3.Ver info");
}


void elegirEquipo()
{
  while(cantEquipo<3)
  {
    CLEARLCD
    CURSORLCDINICIO
	lcd.print("1SER-2BLA-3ARC");
    CURSORLCD2FILA
    lcd.print("4GOO-5SYL-6MEL");
    key = ESPERARTECLA
    int num =key-'0';
    if(num>0&&num<7)
    {
      CLEARLCD
      CURSORLCDINICIO
      lcd.print(num);
      lcd.print("-");
      lcd.print(POKEOPCIONES[num-1].getNombre());
      CURSORLCD2FILA
      lcd.print("Confirmar PKM #");
      key=ESPERARTECLA
      if(key=='#')
      {
        EQUIPO[cantEquipo]=POKEOPCIONES[num-1];
        cantEquipo++;
      }
    }
    TEAMREADY=true;
    CLEARLCD
  }
}

void recibirDatos(int k)
{  
  short i=0;
  volatile byte x1, x2;
  
  if(INICIO)
  {
  	while(Wire.available())
 	{CODE=Wire.read();}
    if(CODE==3)
    {RIVALELIGIO=true;}
  
  }else{
    
    if(Wire.available() > 0)
    {CMD = (char) Wire.read();}
  
   if(CMD == 'Y')//Informacion del pokemon rival que combate
   {
     i=0;
     while(Wire.available()>0 && i<=16)
     {msj[i++]= (char) Wire.read();}
    }
    else if (CMD == 'C')//continuar proximo turno
   	{hayJugadorDerrotado=false;}
    else if(CMD == 'E')//fin
      {hayJugadorDerrotado=true;}
    else if(CMD =='R')//danio rival
    {
      x1 = Wire.read();  
      x2 = Wire.read();  
      danioRival = (int)x1 << 8 | (int)x2;
      RIVALELIGIO=true;
    }
  }
}

void enviarDatos()
{
  String respuesta; 
  if(INICIO)
  {
  	Wire.write(cantEquipo);
  }
  else  {
    if(Wire.available() > 0)
    {CMD = (char) Wire.read();}
  
    if(CMD == 'I')//Maestro pide informacion del poke que combate
    { 		respuesta = EQUIPO[POKEACTIVO].getNombre();
    		respuesta += ":";
    		respuesta += EQUIPO[POKEACTIVO].getSalud();
    		respuesta += "HP-";
    		char charResp[16];
    		respuesta.toCharArray(charResp,16);
     		Wire.write(charResp);
    }else if (CMD == 'V')//Maestro pregunta cuantos pokemones vivos
    {
      Wire.write(cantEquipo);
    }else if (CMD == 'D')//Maestro pregunta decision del turno
    {
      Wire.write(highByte(danio)); 
      Wire.write(lowByte(danio));
    }
  }
    
}