/*********************************************************************************************/

/* ========== INCLUSAO DAS BIBLIOTECAS NECESSARIAS ================= */
/* DISPONIVEIS NO GITHUB */
#include <AsyncWebSocket.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>

#define RXp2 16
#define TXp2 17

/* ============= DEFINICAO DE CONSTANTES ============== */

// PINO INDICACAO DA CONEXAO
#define Pin_LED_CONNECTION 4

// PINO INDICACAO DO FUNCIONAMENTO DA PLACA
#define Pin_LED_BOARD_STATUS 2

// Tempo Sincronia Serial
#define TempoSincroniaRX 1000

#ifdef Pin_LED_BOARD_STATUS
  #define STATUS_OK 1
#endif

/* ============= DECLARACAO DE VARIAVEIS =============== */ 

// ########### DADOS DA CONEXAO #############

//PREENCHA COM O LOGIN DA SUA REDE
const char* LOGIN = "YOUR_SSID_HERE";

//PREENCHA COM A SENHA DA SUA REDE
const char* SENHA = "YOUR_PASSWORD_HERE";

// TORNAR IP DA PLACA FIXO - ESCOLHER IP
IPAddress local_IP(192, 168, 1, 1);

// IP DA REDE EM USO - SELECIONAR
IPAddress gateway(192, 168, 1, 2);

// MASCARA DE REDE
IPAddress subnet(255, 255, 255, 0);

// ########### VARIAVEIS DO SISTEMA ############

// CONTADOR DE TEMPO DE RECONEXAO APOS PERDER CONEXAO
int cont_reconnect = 0;

//TEMPO TIMEOUT DE RECONEXAO 
//Em segundos (SELECIONE O DESEJADO). 3600s = 1h; 86400s = 1d
int long TimeOut = 3600;

// CASO A CONEXAO FALHE E NAO CONSIGA RECONECTAR APOS TimeOut, Disconnect = 1.
bool Disconnect = 0;

// BAUD RATE PARA COMUNICACAO SERIAL
long PARAMS_SERIAL = 115200;

// BUFFER PARA RECEBER DADOS SERIAIS DO RADIO
String BUFFER_RECEIVE = "";

// BUFFER PARA ENVIAR DADOS PARA O CLIENTE
String BUFFER_SEND = "";

// DADOS DIVIDIDOS EM MAX 7 PARAMETROS
String data_1 = "";
String data_2 = "";
String data_3 = "";
String data_4 = "";
String data_5 = "";
String data_6 = "";
String data_7 = "";

// ####### APLICACAO SERVIDOR-CLIENTE SOCKET ########## 

// Objeto AsyncWebServer na porta 80

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// PAGINA HTML PARA ACIONAMENTO DO FREIO

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>

<style>

/* CSS */

h1 {
  text-align: center;
  font-family: 'Overpass';
  font-style: normal;
  font-weight: bold;
  font-size: 40px;
  letter-spacing: -0.4px;
  color: white;
}

.topnav {
overflow: hidden;
background-color: #336571;
box-shadow: #203F47 0 -12px 6px inset;
}

.button {
  appearance: none;
  background-color: #83D1E4;
  border-radius: 40em;
  border-style: none;
  box-shadow: #336571 0 -12px 6px inset;
  box-sizing: border-box;
  color: #000000;
  cursor: pointer;
  display: block;
  font-family: "Lucida Console", "Courier New", monospace;
  font-size: 30px;
  font-weight: 600;
  letter-spacing: -1.0px;
  margin-left: auto;
  margin-right: auto;
  outline: none;
  padding: 1rem 6rem;
  width: 90%;
  quotes: auto;
  text-align: center;
  text-decoration: none;
  transition: all .15s;
  user-select: none;
  -webkit-user-select: none;
  touch-action: manipulation;
}

.button:hover {
  background-color: #FFC229;
  box-shadow: #FF6314 0 -6px 8px inset;
  transform: scale(1.05);
}

.button:active {
  transform: scale(1.025);
}

@media (min-width: 768px) {
  .button-73 {
    font-size: 1.5rem;
    padding: .75rem 2rem;
  }
}

#footer {
overflow: hidden;
background: #336571;
bottom:0;
height:60px;   /* Height of the footer */
color: white;
clear: both;
margin-top: 60px;
}

</style>

<body style = "background-color: Lightgrey; margin: 0;">

<div class = "topnav">
<h1> DADOS </h1></div>

</br>

<!-- HTML !-->

<button class="button" role="button"><span id = "data_1"></span></button>
</br>
<button class="button" role="button"><span id = "data_2"></span></button>
</br>
<button class="button" role="button"><span id = "data_3"></span></button>
</br>
<button class="button" role="button"><span id = "data_4"></span></button>
</br>
<button class="button" role="button"><span id = "data_5"></span></button>
</br>
<button class="button" role="button"><span id = "data_6"></span></button>
</br>
<button class="button" role="button"><span id = "data_7"></span></button>

</br>

<div id="footer"></div>

<script>

var gateway = `ws://${window.location.hostname}/ws`;
var websocket;

var data_1;
var data_2;
var data_3;
var data_4;
var data_5;
var data_6;
var data_7;

AtualizarDados();

window.addEventListener('load',onLoad); //quando a pagina carregar (load)

function onLoad(event){
  initWebSocket(); //INICIALIZAR A CONEXAO WEBSOCKET
}

function initWebSocket(){

  console.log('Tentando abrir uma conexao websocket...');
  websocket = new WebSocket(gateway);
  websocket.onopen    = onOpen;
  websocket.onclose   = onClose;
  websocket.onmessage = onMessage;
  
}

function onOpen(event) {
  console.log('Conexao Estabelecida.');
}
  
function onClose(event) {
  console.log('Conexao Encerrada.');
  setTimeout(initWebSocket, 2000 
  /*2 segundos para reconectar*/);
}
  
function onMessage(event) {
  var BUFFER_DATA;
  BUFFER_DATA = JSON.parse(event.data);
  
  data_1 = BUFFER_DATA.data_1;
  data_2 = BUFFER_DATA.data_2;
  data_3 = BUFFER_DATA.data_3;
  data_4 = BUFFER_DATA.data_4;
  data_5 = BUFFER_DATA.data_5;
  data_6 = BUFFER_DATA.data_6;
  data_7 = BUFFER_DATA.data_7;
  
  AtualizarDados();
}

function AtualizarDados(){

  document.getElementById("data_1").innerHTML = data_1;
  document.getElementById("data_2").innerHTML = data_2;
  document.getElementById("data_3").innerHTML = data_3;
  document.getElementById("data_4").innerHTML = data_4;
  document.getElementById("data_5").innerHTML = data_5;
  document.getElementById("data_6").innerHTML = data_6;
  document.getElementById("data_7").innerHTML = data_7;
}

</script>

</body>
</html>
)rawliteral";


void SendClients() {
  // Funcao para enviar dados aos clientes.
  ws.textAll(BUFFER_SEND);
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
   // FUNCAO HANDLE WEB SOCKET EVENT - SERVIDOR           
   switch (type) {
    case WS_EVT_CONNECT: //Quando novo cliente loga
      break;
    case WS_EVT_DISCONNECT: //quando um cliente desloga
      break;
    case WS_EVT_DATA: // quando dados sao recebidos do cliente
      break;
    case WS_EVT_PONG: // resposta do cliente para um request ping
      break;
    case WS_EVT_ERROR: // evento erro
      break;
  }
}

void initSocket() {
  // INICIALIZACAO DO SOCKET
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void SHOW_DADOS_CONEXAO(){

  // Printar dados da conexao
  Serial.println();
  Serial.println("IP DA PLACA/ENDERECO MAC: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());
  Serial.println("HTTP/1.1 200 OK");
  Serial.println("Content-type:text/html");
  Serial.println("Connection: close");
  Serial.println();

}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : ""; 
}

void setup(){

  // INICIAR MONITOR COMUNICACAO SERIAL
  Serial.begin(PARAMS_SERIAL);
  
  // PARA COMUNICACAO SERIAL COM STM32
  Serial2.begin(PARAMS_SERIAL, SERIAL_8N1, RXp2, TXp2);
  

  // GPIO PIN SET
  pinMode(Pin_LED_BOARD_STATUS, OUTPUT);
  pinMode(Pin_LED_CONNECTION, OUTPUT);

  digitalWrite(Pin_LED_BOARD_STATUS, LOW);
  digitalWrite(Pin_LED_CONNECTION, LOW);

  if (STATUS_OK){
    digitalWrite(Pin_LED_BOARD_STATUS, HIGH);
  }

  delay(2000);
  Serial.println("Configurando IP fixo da placa...");
  
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Falhou na configuracao de IP fixo.");
  }
  
  // CONEXAO A WIFI
  WiFi.begin(LOGIN ,SENHA);
  Serial.println();
  delay(1000);
  Serial.println("Conectando a ");
  Serial.print(LOGIN);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  SHOW_DADOS_CONEXAO();

  digitalWrite(Pin_LED_CONNECTION, HIGH);

  initSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Start server
  server.begin();
}

void loop() {

  static bool ToggleStatus = 0;
  ToggleStatus = !ToggleStatus;

  if (STATUS_OK == 1){
    digitalWrite(Pin_LED_BOARD_STATUS, ToggleStatus);
  }

  if (Serial2.available() > 0){

    BUFFER_RECEIVE = Serial2.readString();
    Serial.println(BUFFER_RECEIVE);

    data_1 = getValue(BUFFER_RECEIVE,',', 0);
  	data_2 = getValue(BUFFER_RECEIVE,',', 1);
  	data_3 = getValue(BUFFER_RECEIVE,',', 2);
  	data_4 = getValue(BUFFER_RECEIVE,',', 3);
  	data_5 = getValue(BUFFER_RECEIVE,',', 4);
  	data_6 = getValue(BUFFER_RECEIVE,',', 5);
  	data_7 = getValue(BUFFER_RECEIVE,',', 6);

    BUFFER_SEND = "{\"data_1\":";
    BUFFER_SEND += data_1;
  	BUFFER_SEND += ", \"data_2\":";
  	BUFFER_SEND += data_2;
  	BUFFER_SEND += ", \"data_3\":";
  	BUFFER_SEND += data_3;
  	BUFFER_SEND += ", \"data_4\":";
  	BUFFER_SEND += data_4;
    BUFFER_SEND += ", \"data_5\":";
  	BUFFER_SEND += data_5;
    BUFFER_SEND += ", \"data_6\":";
  	BUFFER_SEND += data_6;
    BUFFER_SEND += ", \"data_7\":";
  	BUFFER_SEND += data_7;
    BUFFER_SEND += "}";

    Serial.println(BUFFER_SEND);

  }

  SendClients();

  delay(TempoSincroniaRX);

  // GARANTIR A RECONEXAO CASO DESCONECTE 
  if (WiFi.status() != WL_CONNECTED && Disconnect != 1){
  digitalWrite(Pin_LED_CONNECTION, LOW);  
  cont_reconnect = 0;  
  // CONEXAO A WIFI
  WiFi.begin(LOGIN , SENHA);
  Serial.println();
  Serial.println("Conexao perdida. Tentando reconectar...");
  delay(1000);
  Serial.println("Conectando a ");
  Serial.print(LOGIN);
   while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    cont_reconnect++;
    if (cont_reconnect > TimeOut){
      Serial.println();
      Serial.println("TimeOut ultrapassado. Sem conexao.");
      Serial.println("Verifique a conexao. APERTE RESET OU DESLIGUE A PLACA E TENTE NOVAMENTE.");
      Disconnect = 1;
      break;
    }  
  }

  if (cont_reconnect < TimeOut){
  digitalWrite(Pin_LED_CONNECTION, HIGH);
  Serial.println();
  Serial.println("Tempo desconectado: ");

  if (cont_reconnect < 60){
    Serial.print(cont_reconnect);
    Serial.print(" segundos. ");  
  }
  else if (cont_reconnect >= 60 && cont_reconnect < 3600){
    Serial.print(cont_reconnect/60);
    Serial.print("m e ");
    Serial.print(cont_reconnect%60);
    Serial.print("s. ");
  }
  else if (cont_reconnect >= 3600){
    int horas = cont_reconnect/3600;
    int resto = cont_reconnect%3600;
    Serial.print(horas);
    Serial.print("h ");
    Serial.print(resto/60);
    Serial.print("m e ");
    Serial.print(resto%60);
    Serial.print("s. ");
  }
  SHOW_DADOS_CONEXAO();
  }
 }

}

/**********************************************************************************************
  
***********************************************************************************************/
