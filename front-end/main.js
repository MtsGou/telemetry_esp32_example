/*script*/

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
