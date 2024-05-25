#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>

String never_gonna[] = {
  "guest wifi",
  "guest wifi",
  "guest wifi",  
};
const int update_interval = 70000; // SSID update interval in ms

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
WebServer webServer(80);

String responseHTML = 
                      "<!DOCTYPE html><html lang='en'><head>"
                      "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>"
                      "<meta name='viewport' content='width=device-width'>"
                      "<title>Rickrolling the WiFi</title></head><body>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡀⠀⠀⠀⠀⠀⠀⢀⡤⠖⣛⣿⣏⠝⡙⠶⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⠀⠀⠀⠀⠀⣠⠶⢶⠲⣄⡴⢻⡍⠩⠳⣤⡀⠀⠀⠚⣉⣴⣋⣁⡀⢸⡙⠼⡑⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⠶⠺⢿⣤⡾⣿⡉⠙⢳⣤⡀⠀⣿⠌⢺⢒⣿⣷⠛⣗⡘⣄⣿⣿⠄⢀⡴⠛⣹⣷⡟⠛⢻⢇⢊⠆⢽⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⠶⢛⣯⡿⠛⠿⠶⣤⡀⠀⠀⠀⠀⣠⠶⢻⣹⡿⠿⢶⣄⡀⠀⠀⡇⢌⠀⢼⣿⡾⠋⡇⢱⠀⣿⣿⠀⣿⠂⠢⢘⣿⢿⠀⣟⠐⢀⢽⣿⡇⣾⠁⣁⢺⣿⡇⠀⢸⠠⢊⠠⢺⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠏⣠⠆⢸⣿⣿⠀⠀⠀⠀⠙⢢⡀⢠⠞⢐⡰⣸⣿⡇⠀⠀⠘⡷⡄⠀⣷⠀⠊⢸⣿⡇⠀⡟⠘⠀⣻⣿⡆⢸⠠⠁⠢⣟⣿⠀⣯⠈⢀⢼⣿⡇⠸⣖⠀⢺⣿⡇⠀⣸⡇⢂⣡⣻⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣤⣄⠀⣀⣤⢶⣤⣄⡀⠀⠀⠀⠀⠀⢰⠇⡄⠠⡐⠸⣿⣿⠀⠀⠀⠀⠀⠀⠠⡟⠀⠜⠜⠸⣿⡇⠀⠀⠀⢻⢹⠀⣿⠀⠂⢼⣿⣧⠀⡇⠌⠠⢹⣿⡇⢸⠐⡀⠂⣿⣿⠀⢷⡀⣆⣸⣿⣇⠀⠈⠳⣼⣿⣷⣋⡭⠟⠿⠿⠷⠿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⢀⣠⣦⡶⣶⠶⢤⡀⠀⠀⠐⡯⠀⠇⣻⣿⣯⡾⠛⠉⠉⠀⠀⠀⠀⠀⠀⢸⡠⠀⠂⠑⠀⣿⣿⠒⢶⣿⡿⠽⡿⢿⢯⠀⢂⠊⠀⣿⣿⠀⠀⢀⡟⢸⠁⣿⠀⠠⢨⣿⣟⠀⣯⢐⣈⣾⣿⡇⠘⢷⣶⣿⣿⣿⠀⠈⠹⠿⠿⠿⠋⠀⠀⠀⠀⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⡤⠤⠶⠦⣄⠀⠀⠀⠀⡼⢁⡦⡴⢋⠀⣿⣿⢋⡇⠀⡏⣷⣄⠀⡏⠘⠔⣻⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣧⠀⠂⢈⠀⣿⣿⠀⠀⠀⠀⢠⠇⣾⠘⢦⡁⠈⢀⣿⣿⠀⣠⠞⣠⠏⠀⠹⢴⣶⣾⣿⡿⠀⠈⠹⠿⠿⠟⠃⠀⠀⠉⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⡖⠦⣀⠀⠀⠀⠀⠀⠀⢸⡁⡇⠀⠀⣠⠔⢾⣋⣿⡏⠗⢦⡀⠀⠳⣄⠰⢀⠽⡷⡀⠀⣸⢃⡾⣼⠁⡠⠀⣿⣿⣀⣿⣨⣤⢾⣻⣯⡧⠈⢐⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢷⡀⢀⠂⣿⣿⠀⠀⢀⡴⢋⣴⠃⠀⠀⠙⠾⣣⣿⣿⣚⣥⠖⠁⠀⠀⠀⠈⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣤⡴⣶⢶⣶⣆⡀⠀⠀⠀⠀⠀⣿⢰⡆⠀⣀⣀⣤⣦⣦⠀⣠⠶⠿⣿⣟⣿⣳⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡇⢤⠈⠙⠒⠤⣀⠀⠀⢸⡇⣇⢠⡞⢡⠄⣿⣿⠁⣿⡰⠄⢹⣷⡀⠹⡤⠤⠄⢿⣽⣤⠇⣼⠡⣇⠠⠑⠀⣿⣿⠿⠓⠛⠛⠛⠉⠁⡇⠈⠁⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠳⢶⣻⣿⣲⣾⡽⠾⠋⠁⠀⠀⠀⠀⠀⠀⠀⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣻⣼⢳⣟⡷⣿⣿⣿⠀⠀⠀⠀⠀⣿⢸⡇⢸⣻⡽⣷⣭⣿⣾⣇⣵⠟⢹⣿⢾⣷⣛⣿⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⡇⢨⠢⠑⡀⢀⠄⠉⠲⣬⡇⣿⡟⠀⠉⠑⣿⣿⢤⡽⢷⣖⣖⣯⣿⡀⠘⣆⠀⠠⢀⣾⣴⠇⠀⠹⣄⠂⠌⣿⣿⠀⠀⠀⣰⠋⣰⠂⢷⣁⣠⡼⣿⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣷⣻⢾⣿⣝⣯⣽⣿⣿⠀⠀⠀⠀⠀⣻⢸⡇⢸⣳⢟⡷⣯⢿⣿⡟⠁⠀⢸⣿⣻⣟⣿⢳⡾⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⡗⣶⣂⠁⠈⠔⠘⠂⠐⠘⣿⣿⣯⠀⠌⠀⢽⣿⡋⠉⠉⠉⢉⠤⡄⠀⠀⠘⣆⠈⣞⣿⠏⠀⠀⠀⠈⠳⢤⡼⣿⣤⡴⢊⣥⠞⠁⠀⠈⠹⠿⠛⠛⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⠀⠀⠀⠀⠀⢠⡶⢶⡞⡿⣧⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⣯⢿⢯⣟⣾⣳⢽⣿⣿⠀⠀⠀⠀⠀⢹⣸⡇⢸⢯⣻⣯⠽⣾⣿⡇⠀⠀⢸⣷⣻⣿⣺⣏⢿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⢿⠈⠙⠦⡆⠀⠈⠁⠈⡿⣿⠘⢧⠈⠀⢸⣿⡇⠀⠀⡴⠋⣴⠏⠀⠀⠀⠈⢿⣽⡏⠀⠀⠀⠀⠀⠀⠀⠉⠙⠛⠋⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡼⠁⣴⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢴⠻⢯⢻⡭⢿⣤⠀⠀⠀⠀⢸⢿⣽⡽⣷⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⡟⣿⢻⡜⣿⡏⢯⣿⣿⠀⠀⠀⠀⠀⢸⢸⡇⢸⣯⢷⢻⡟⣾⣿⡇⠀⠀⢸⡷⡏⡿⡷⣯⣾⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⢸⡇⠀⠀⠉⠚⠷⣀⡂⡧⣿⠀⠈⠳⢤⣎⣿⡧⡴⣞⣡⡾⠋⠀⠀⠀⠀⠀⠈⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⣶⠶⣶⡺⣻⢟⣻⢦⡀⠀⠀⠀⠀⠀⣼⠃⣰⠏⠀⣠⡴⣶⣿⢛⣛⣲⣶⣄⡀⠀⠀⠀⣾⡳⣮⢿⡗⣾⣿⡇⠀⠀⠀⢸⣞⣯⣷⢯⣟⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⣿⣛⢾⣻⣽⢻⣜⣻⣿⡀⠀⠀⠀⠀⢸⢸⡇⢸⣟⣮⣟⣿⢽⣿⡇⠀⠀⢸⡷⣻⢷⢧⣻⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣾⡇⠀⠀⠀⠀⠀⠈⠙⠳⠿⠀⠀⠀⠀⠉⠙⠛⠋⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⡲⢿⣟⣾⣷⡯⡽⣦⡀⠀⠀⡼⠃⣰⠏⣰⢯⠳⣝⢲⣻⣿⡍⠉⠉⠛⠻⣦⡀⠀⣿⢻⣭⢾⢿⣽⣿⡇⠀⠀⠀⢸⢾⣾⢻⣳⡯⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⢿⣽⣻⢧⣻⡝⣮⢽⣿⡇⠀⠀⠀⠀⡞⢸⡇⠘⣿⣼⣻⣞⣿⡿⡇⠀⠀⢰⡿⣝⣯⣾⣿⣿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠹⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢷⣚⢷⣏⣟⡷⣣⠻⣦⡼⠁⣼⠇⣼⣏⡞⣟⡺⣽⣻⣿⠁⠀⠀⠀⠀⢹⣻⡄⢹⣗⣯⢯⣯⣿⣿⡇⠀⠀⠀⢸⣏⣾⣻⣥⡟⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠘⣷⢯⢿⣱⢯⣟⣾⣿⡇⠀⠀⣠⠞⢁⡾⢑⠀⣿⣽⢾⣻⣼⣿⡿⣷⣾⠿⣻⣿⣿⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⡴⣦⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡄⠀⠀⠀⠀⢀⣤⣶⠛⢛⣻⣟⣲⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣞⡼⣸⡟⣭⣳⣽⣿⣾⠃⢰⡗⣮⡝⣮⣿⢼⣻⣿⡀⠀⠀⠀⠀⢸⡏⣧⢸⡞⣯⡻⣞⡻⣿⡇⠀⠀⠀⠀⡿⡽⢮⣝⡳⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣷⣏⣷⣫⣾⣿⡧⠖⠋⣁⣤⠞⠁⠐⠀⣿⣽⢯⣗⢿⣿⣯⠀⠉⠙⠛⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠘⠿⣷⣿⣥⣿⣶⡄⠀⣀⣀⣀⣄⣀⡀⠀⠀⠀⠀⠀⠀⣰⠃⣰⠀⢀⣞⡯⢳⣽⣿⡏⢉⡷⡳⢮⡝⣦⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢷⣹⢼⣳⣾⣿⡿⠃⠀⠸⣟⣣⢟⣣⡿⢾⣹⣿⡇⠀⠀⠀⠀⣸⠀⣿⠘⣿⡱⣿⣱⢳⣿⡧⠀⠀⢀⡴⣿⣹⠻⣜⣳⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠻⠿⠿⠿⠷⠚⠛⠉⠀⠀⠀⠀⠀⣿⣾⣽⣎⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⣠⣴⠾⣿⣉⣭⣭⣽⣽⣗⣦⡀⠀⠀⠀⢀⡤⣼⣯⠿⣿⡏⠁⠈⢻⡼⣹⣮⡟⣽⣦⡀⠀⠀⠀⢠⡏⢠⡏⢠⢿⣚⡼⣣⣿⣿⡇⠈⣷⢻⣻⡼⣪⢽⣦⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣀⣀⠀⠀⢹⣶⣿⣿⣿⣡⣤⣤⣄⣿⡖⣯⢓⣾⡹⢮⣿⡇⠀⠀⠀⣰⠏⣰⡏⠀⠙⢷⡭⣷⣛⡼⣷⣠⠔⠋⣀⡿⢶⣿⣾⣷⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢸⡿⠿⠿⠛⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⣰⡟⣳⢭⣟⡶⣿⡇⠀⠀⠀⠀⠉⠛⢷⣤⡀⢸⣭⢷⢾⣏⢷⣿⣷⠀⠈⢻⡵⣫⣿⢦⠷⣷⡄⠀⢀⡟⢠⡿⠄⣟⢧⣛⡖⣏⣿⣿⡧⠤⠿⢿⣶⣷⣿⣿⣿⠇⠀⠀⠀⢀⣠⣴⠞⢯⢉⢇⣽⢿⡿⠻⠋⡼⣛⣿⡇⡶⠓⣴⡶⢦⠙⢮⣝⠮⣝⣛⣿⡇⢀⣠⢞⢁⣴⠟⠀⠀⠀⠀⠙⠿⣿⣿⣿⣷⠶⠞⠉⠀⠘⠛⠋⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⣼⢏⡾⣝⣿⢼⣹⣿⡧⠀⠀⠀⠀⠀⠀⠐⠙⠃⢸⢮⡻⢾⣞⡺⣿⣿⠀⠀⠀⢻⣵⢺⣛⣽⡸⢿⡄⡾⠰⣾⠁⠸⣏⣿⣱⠿⡸⣿⣿⡗⠖⠒⠛⠋⠉⠉⠁⠀⠀⠀⠀⣴⣯⡟⣤⡙⢪⣼⣿⢓⢺⡇⠀⡼⠁⣼⣧⣾⣧⣼⣿⣮⡀⠻⣦⡛⢿⣾⣿⣿⣟⣉⣷⣶⠟⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⢸⢯⣽⣹⡮⣝⣺⣝⣿⣿⠀⠀⠀⠀⠀⣀⣀⡀⠀⢸⡧⣿⣯⡞⣽⣿⣿⠀⠀⠀⠀⠻⡧⡟⣦⠻⡵⣻⣇⣸⠃⠀⠀⢻⣮⣝⠟⣧⡹⣿⡇⠀⠀⠀⠀⢀⡜⠁⣤⣤⣖⣿⢳⣿⡖⡤⡙⢆⢿⣿⣯⡦⣻⣼⠁⣼⠃⠀⢹⣿⣿⣿⣷⣯⣶⡿⣿⣿⢦⡉⣯⢉⣽⠟⠾⠳⢤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⣿⣳⡟⣾⡹⣏⣶⣹⣿⣿⠐⠒⢛⣛⣉⣭⣤⣿⢦⣙⡿⣜⡽⠾⣍⣿⣿⠀⠀⠀⠀⠀⠹⡽⣜⢫⡵⣾⣿⡟⠀⠀⠀⠀⠙⢞⢿⣜⣹⠻⡇⠀⠀⣀⡴⠋⢀⡾⣷⠟⠐⣼⣗⡫⢿⠿⣷⣯⣭⣿⡿⣿⡿⣯⣍⡡⠘⣷⣮⣽⣿⣿⣿⡿⣿⣿⣿⣿⣧⣷⣬⣿⣯⡒⢨⣼⣻⣽⡷⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠹⣷⢟⣳⡝⣮⡝⡾⣿⣿⠀⠈⠉⠉⠁⠀⢠⠇⣸⠟⡿⣼⢱⡫⣇⢿⣿⠀⠀⠀⠀⠀⠀⠙⣞⡧⣽⣿⡿⠀⠀⠀⠀⠀⠀⠀⠙⠾⣯⣽⣾⣗⣋⣁⣤⠶⠋⠠⣿⡮⣔⠹⣿⣿⣟⣿⣿⣟⣿⣿⣿⣿⡛⣷⣿⣻⣮⡬⢿⣿⣿⣻⣿⣾⣿⣿⣟⣿⣿⣿⢿⣻⣿⣯⡣⡽⣿⣿⣻⣷⡽⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠹⣯⢗⡻⣖⡻⢼⣹⣿⠀⠀⠀⠀⠀⣠⡞⢀⡟⠄⣟⢦⣓⡳⡼⢹⣿⠀⠀⠀⠀⠀⠀⠀⠈⠻⣿⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠋⠉⠀⠀⠀⣠⣾⣿⣿⣽⣾⣷⣯⣻⡿⢿⣮⣿⢿⣯⣿⣟⣻⣏⢿⣷⡨⣿⢿⣯⢿⡿⣎⢿⣿⣾⣿⣏⣿⣿⣿⣿⣿⣯⣻⣿⣿⣿⣿⣿⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠙⢾⣵⣫⢵⢫⣼⣿⠀⠀⣀⣠⠾⠋⣢⡞⠁⠀⠛⢳⣭⣾⣷⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣶⣯⣿⣿⢳⣍⡻⣿⣿⣿⣿⣻⣿⣿⣿⢿⢿⣟⣿⢾⣿⡾⣽⡇⡼⣿⣿⣻⡗⣯⣿⣿⢯⣽⡷⣏⣿⡿⢿⣾⣻⣿⣿⣷⣻⣿⣿⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠙⠻⣮⣷⣽⣿⡖⢛⣉⣠⡴⠞⠋⠀⠀⠀⠀⠈⠉⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⡿⣳⣿⣿⣿⠋⡜⣿⣿⣿⣿⣿⣿⣯⡻⣽⣻⣿⣿⡏⣿⣎⣿⣗⣼⢇⣟⣾⡿⣿⣹⣻⣟⣿⢺⣾⣻⣿⢻⡜⣻⣶⣿⢽⣿⣿⣿⢿⣿⣾⣿⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠋⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡇⢆⡽⣿⣿⣻⣿⡖⣠⣿⣿⣿⣿⡽⣿⣿⡗⣽⣻⡿⣹⡟⣬⡿⢦⣟⣺⣿⢫⣿⢳⡵⢿⣹⢣⣿⣛⣾⢯⣏⣿⣿⢿⣻⣿⣽⣿⢧⣯⣿⣗⣾⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣷⣞⣷⣝⠟⣛⣋⡕⣱⣿⣻⡧⠿⠿⠟⠛⠛⠛⠛⠛⠳⠿⠿⠷⣿⡼⡿⠿⠟⠛⠛⠛⠛⠛⠛⠛⠛⠛⠻⠿⠿⣿⣿⣽⣺⢟⣽⣿⢿⣿⣿⡿⣯⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣷⣽⡿⢿⢿⣿⣿⣿⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣿⣞⣯⣷⣟⣽⡿⣵⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⡟⡹⣿⣿⣭⣾⣺⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⢿⢾⣻⣽⣼⡿⢿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⢧⣕⡷⣿⣿⡿⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⣮⣯⣙⣾⣽⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⢌⠻⣟⣿⣻⣽⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⣿⣮⣻⢿⡾⡿⢿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣘⣯⣿⣽⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣝⣿⣿⢿⡽⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣇⢻⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣞⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢽⣿⣽⣿⣿⣯⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⡹⣷⡿⣿⢿⡇⠀⠀⠀⠀⠀⠀⠀⣀⣤⣤⣴⣴⣶⢷⡶⣶⣶⠆⠀⠀⠀⠀⠀⠀⢿⠴⣷⣶⢦⣄⣀⣀⠀⠀⠀⠀⠀⠀⠀⣿⡿⣽⣫⣽⡾⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⣿⡵⣦⣿⣿⣿⠃⠀⠀⠀⠀⣠⣴⣿⣿⢿⢿⣧⠷⠾⠷⠿⠛⠁⠓⢄⠀⠀⠀⠀⠀⠀⠀⠈⠻⢿⣽⣭⣟⡿⡷⣶⣤⣀⠀⠀⠛⣿⣧⣽⡯⣽⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⡕⣻⣿⡿⣿⠀⠀⠀⢰⣾⠟⠋⢁⡴⠚⠉⠉⠓⢶⢆⠀⡀⠀⠀⠸⢳⡀⠀⠀⠀⠀⠀⠀⣠⣴⠆⠉⠛⢯⡉⠉⠛⢿⣳⡄⠀⢿⣿⣽⣿⡿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣷⢺⣿⣿⢿⡄⠀⠀⠉⠁⠀⣴⣏⣤⣴⣶⣶⣮⢤⣟⢤⢹⡖⠀⠀⠀⡇⢀⠰⠲⢰⣯⠼⣟⣥⣴⣶⣤⣤⣙⢦⡀⠀⠈⠻⠀⣸⣿⣿⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣮⣿⣿⣿⠀⠀⠀⠀⠀⠔⠛⠻⠧⣽⣷⣾⣿⠥⣿⡿⠘⠁⠀⠘⠃⡧⠘⠂⠀⠘⠛⢾⠻⢼⣿⣾⣿⡤⠿⠛⠶⠀⠀⠀⠀⣿⣽⣾⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡤⡤⢤⣧⣽⣯⡿⠀⠀⠀⠠⠄⠀⠀⠀⠀⡀⠀⠀⠀⠛⠁⠀⠀⠀⠀⣠⣼⡟⠀⠀⠰⠄⠀⠀⠙⠒⠀⡀⠀⠀⠀⠀⠄⠀⠀⠀⠠⣿⣿⡧⠤⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⡟⢀⣤⣄⡀⠙⢿⡇⠀⠒⠀⠀⠀⠀⠀⢀⠀⠀⠂⠀⠀⣀⠀⠉⠀⠀⠊⠈⣿⣿⠘⠋⠀⠀⢀⡀⡠⡤⠀⠁⠀⠀⠀⠀⠀⠀⠐⠀⠈⣿⢉⣠⢤⡈⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢽⠁⣾⠁⠀⠙⢦⡘⡇⠀⠀⠀⠀⠃⠀⠀⠀⢀⠀⠀⠀⠀⠉⠀⠀⢀⡠⠶⠀⠁⠀⠀⠀⠀⠠⣏⠀⠀⠀⠀⢀⠈⠀⠀⠀⠃⠀⠀⠀⠈⡿⠋⠁⢨⡇⢸⡂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣆⢹⡠⠶⠶⢶⡿⣷⠀⠀⠀⠀⠀⠀⠠⠀⠀⠀⠀⠁⠀⠀⠀⢰⠏⣴⣴⣀⠀⠀⠀⠀⣀⣀⠈⣷⡄⠀⠀⠀⠀⠠⠀⠀⠀⠀⠀⠀⢸⣿⠶⠶⢪⠃⣼⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣄⣷⡀⠀⢠⡇⢻⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠏⠳⠽⠟⠛⢧⣀⣠⠞⠻⠽⠿⠃⢽⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠛⡆⢠⠏⣼⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣮⣇⠀⠀⠳⣼⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⠃⠀⠀⠀⠀⠀⣴⠋⢥⠀⠀⠀⠀⠀⠀⠹⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠜⢁⡏⡼⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣃⠀⠀⠀⢻⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⠃⠀⠀⠀⠀⠀⠙⠁⠀⠹⠗⠀⠀⠀⠀⠀⠀⢻⡄⠀⠀⠀⠀⠀⠀⠀⢸⠃⠀⠨⣼⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⡖⣀⠀⢈⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⣶⠇⠀⠀⢀⣠⠴⠾⡓⠒⠒⠒⠶⠤⣄⠀⠀⠀⠀⠈⣷⡀⠀⠀⠀⠀⠀⢠⡏⠀⣢⠞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠓⠒⠛⢹⣇⠀⠀⠀⠀⠀⠀⠀⡐⠉⠀⣠⢖⡯⢖⡒⠉⠉⠉⠉⠙⢓⠒⢦⣝⢦⣀⡀⠈⠻⠂⠀⠀⠀⠀⠀⣼⠈⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣄⠀⠀⠀⠀⠀⠀⠀⠀⠻⡿⣿⣿⣿⣿⣿⣿⣿⣶⣾⣿⣿⣿⣿⣿⡿⠁⠀⠀⠀⠀⠀⠀⠀⣼⠣⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⢻⡄⠀⠀⠀⠀⠀⠀⠀⠀⠙⢎⡙⢿⣿⣿⣿⣿⣿⣿⣿⡿⠟⣫⠞⠀⠀⠀⠀⠀⠀⠀⠀⣹⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡇⢻⣇⡀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠦⣄⣈⠉⠉⢉⣉⣡⡴⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⣵⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣇⠠⡹⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⢉⣉⣉⣉⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⢻⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠁⢝⣧⣆⠀⠀⠀⠀⠀⠀⠀⠀⠒⠋⠉⠉⠉⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⣠⡾⡱⢸⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⠀⠀⠈⢎⢿⡷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⡿⠸⠀⢸⣿⣶⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⡖⠀⠀⠀⠈⠻⣯⡦⠀⠀⠀⠀⠀⣀⣤⣤⣤⢦⣤⣦⣤⣤⣤⣤⣤⣀⡾⠋⠀⠀⠀⣿⠙⣿⣻⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⠟⢹⡇⠀⠀⠀⠀⠀⠈⠛⢿⣄⡀⠀⢰⠏⡷⢒⣶⠶⣷⠷⣶⠶⢶⢬⣤⡼⢿⡇⠀⠀⠀⢸⠀⠈⣿⣯⣿⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⠏⠀⠈⡇⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠓⢿⠀⣷⣫⠞⣅⠼⢯⡬⢷⣉⣷⡻⡇⢸⡇⠀⠀⠀⢸⡾⠀⠙⣿⣻⣿⣷⣦⡀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠄<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⣿⠋⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡿⠀⣏⡽⣞⡹⢿⣉⣷⣞⡽⣄⠹⡇⢸⡇⠀⠀⠀⢸⡇⠀⠀⢹⡿⣿⣿⣟⣿⣶⣤⡀⠀⠀⠀⠀⢾⠁⠯⣙⣲⠶⠤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣯⡏⠀⠀⠀⡦⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡇⢠⣇⠴⢋⣶⣖⡹⣖⣹⢖⢉⣧⡇⢸⡇⠀⠀⠀⣾⠀⠀⠀⠈⣿⣿⣿⡿⣿⣿⣿⣿⣷⣦⣄⡀⠘⢧⡀⠀⠀⠠⢏⡜⠉⠓⠦⢄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣤⣶⣾⣿⣿⣿⣽⣿⠀⠀⠀⠰⡧⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡇⢸⡷⣖⣱⣖⣻⣔⢊⣧⡖⣳⣜⡇⢸⡇⠀⠀⢠⡏⠀⠀⠀⠀⢻⣿⣿⣿⣿⣻⣿⣿⣿⣿⣿⣿⣿⣶⣽⣶⣄⣀⠁⠀⠀⠀⠀⠀⠉⡳⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣄⣦⣶⣿⣿⣿⣿⣿⣿⣾⣿⣿⣿⡇⠀⠀⠀⠀⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡇⣸⡔⢉⢶⠋⣦⠞⣫⡔⢛⣄⠘⡇⢸⡇⠀⢄⡞⠀⠀⠀⠀⠀⢸⣷⣿⣷⣿⣿⣿⢿⣿⣿⣿⣿⡿⠿⠿⠿⠿⣿⣷⣤⣀⣀⠀⠨⠯⠖⠻⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣄⣦⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣿⣿⣿⣾⠃⠀⠀⠀⠀⢻⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡇⣽⢘⣧⠔⣻⡔⢻⣥⠛⣧⠼⢻⡇⢸⡇⢢⡟⠁⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⠟⠻⣉⡩⠇⠀⠿⠇⠀⠀⠀⠀⠀⠀⠀⠀⢩⢯⡷⣄⠀⠈⢻⣶⣶⣤⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡁<br>"
"  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣟⣿⣿⣿⣟⣿⠀⠀⠀⠀⠀⠈⣷⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠁⣟⣪⡔⢫⡤⠞⣯⠼⢎⡡⠷⣽⡇⢸⣵⠏⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⢷⣄⣀⣀⣀⣀⣠⣀⣀⠀⠀⠀⠀⠀⠀⠀⠋⣿⠼⠨⢣⡀⠨⣿⣿⣿⣿⣿⣿⣷⣶⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣴⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣽⣿⣿⡿⣿⣿⡏⠀⠀⠀⠀⠀⠀⠘⣷⣆⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⡿⡥⠚⣯⠽⣎⡥⠻⣉⠷⣞⣹⡇⣼⠁⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣻⣿⢿⣿⣿⣯⣿⢿⡻⡟⠛⠉⠉⠉⠉⠛⠛⠛⡖⡦⣄⠀⠀⠀⠓⡄⠸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠄<br>"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣤⣶⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣿⣺⣿⣿⣿⣿⡿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠘⢷⣆⠀⠀⠀⠀⠀⠀⠀⢸⠀⣧⡼⢯⡭⠻⣉⠷⣞⣹⢴⢊⣹⡇⣾⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣽⣿⣿⠟⢻⣉⡹⠀⠉⠳⠟⠀⠀⠀⠀⠀⠀⠀⢸⢹⣻⠋⠳⣄⠀⠀⠈⢦⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠂<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣴⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣛⣯⣿⢿⣷⣟⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠈⢳⣇⡀⠀⠀⠀⠀⠀⢾⠀⣷⡟⣥⡽⢾⣹⢶⣋⣧⡞⣫⢼⡇⡟⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⣿⣿⣿⣷⣤⣥⣀⣠⣤⣼⣶⣶⠦⢤⣀⣀⣀⣀⠀⠀⠁⠀⠀⠈⠳⢄⡀⠀⠙⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣴⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⣾⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢷⣄⠀⠀⠀⠀⢸⣄⡗⠻⠷⠷⠿⠷⠾⠿⠤⠿⠵⠿⣄⡇⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⢿⣿⣿⣿⣿⣿⣻⢿⣿⣿⣿⣿⣀⣄⣀⣀⣉⡉⠳⣄⠀⠀⠀⠀⠀⠀⠙⠢⣄⠨⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣻⣯⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠄<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⡻⣟⣿⢿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⣗⡄⠀⠀⠀⠛⠶⠶⢤⣤⣤⣤⣤⣤⣤⣤⣤⠤⠟⠁⠀⠀⠀⠀⠀⠀⢀⣿⣽⣿⣿⣻⣿⣿⡿⣟⣯⠿⡟⣻⠍⢹⠃⠀⠀⠀⠉⢉⡷⡮⣷⣀⠀⠀⠀⠀⠀⠀⠀⢻⡭⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣿⣿⣟⡻⣟⣿⣻⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠳⣄⠀⠀⠀⢀⡴⠚⠁⣿⠀⠀⢸⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣽⣻⣿⣿⢿⢷⣏⣟⣦⣼⣀⣀⣀⣴⣤⣀⡀⠀⠟⠛⠁⠨⠻⣮⠀⠀⠀⠀⠀⠀⠘⠁⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡫⣞⣛⣽⣮⣟⢿⡿⣿⣯⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠳⣄⡰⠋⠀⢀⡔⠻⠴⠤⠼⠳⡀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣯⣾⣽⣿⣿⢿⣿⣿⣿⣾⢽⡻⣽⣾⣿⡏⠀⣀⠉⠉⢳⡀⠀⠀⠀⠈⠓⣤⡀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⣿⣾⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣟⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⡵⣫⠷⣯⣛⢾⣏⣾⡿⡿⣽⣺⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢷⠶⠶⡟⠓⠶⠤⣥⡤⠴⢻⠀⠀⠀⠀⠀⠀⠀⣠⢿⣿⣯⣿⢿⣾⢿⡽⣟⣻⣷⢷⡾⣟⣿⣟⣿⣿⡇⠀⢸⠀⡆⣉⢣⡀⠀⠀⠀⠀⠀⠈⢳⣄⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣻⣿⣿⡿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⢏⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣕⡚⣿⣸⣏⣟⣷⢿⡿⣏⣵⣷⠀⠀⠀⠀⣀⣀⣠⣤⣤⣶⠶⢶⣾⣿⣏⣉⣉⣉⣉⣀⡀⡗⠀⠉⠀⢥⠀⢰⣾⠃⠀⠀⠀⠀⢀⡼⠃⢸⣳⣧⣿⢯⣞⡿⣿⡷⣿⡿⣟⣷⣿⢷⣾⣿⣿⡇⠀⢸⠀⡇⣿⢠⣿⣄⠀⠀⠀⠀⠀⢠⡏⠀⢄⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣷⣿⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡷⢭⣳⢿⣪⢿⣫⣿⡾⣟⣻⢽⣶⡶⣿⣿⡏⠙⠛⠛⠓⠲⠿⠷⠾⠤⣾⣬⣏⣩⣝⣭⣹⡇⠀⠀⠀⠈⠀⠐⣹⡷⣤⣀⢀⣴⠿⡲⣞⣿⣣⣟⡯⢷⣭⡟⣷⠿⣗⡿⣳⡭⣻⣽⣿⣿⣿⡇⠀⢸⡀⡇⣏⡾⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⢯⣽⣿⣷⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣹⣻⡳⡽⡿⣷⡾⣿⣾⣾⣿⣫⣿⣿⣿⣿⣀⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⡇⠀⠀⠀⠀⠀⢀⢸⠛⠛⠛⠛⠚⠚⠒⠞⢿⣼⣗⣯⣷⣫⢾⢯⣟⣻⣝⡷⣽⣓⢾⣿⣿⣿⡇⠀⠀⣇⢹⡾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣻⣽⣿⣿⣿⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣾⣽⣺⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣶⣻⡞⣕⡿⣷⣯⣟⣿⣮⣿⣯⣷⣿⣿⣿⣗⣮⣍⣯⣛⣛⢻⣒⢲⠶⠶⡶⡶⣤⣤⣤⣿⠀⠀⠀⠀⠀⠐⢽⣀⣀⣀⣀⡀⠀⠀⠀⢸⣟⣿⣵⡫⣷⢯⣻⢾⢧⡿⣽⣣⣝⣻⣿⣿⣿⣇⠀⠀⠘⡾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣿⣿⣿⣿⣿⣿⣿⣿⡁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣾⣻⣿⡾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⢎⣽⠟⣝⢯⣳⠾⢽⣏⡷⣿⡿⣾⣽⣿⡇⠈⠉⠉⠉⠙⠛⠓⠳⠿⠶⠷⠷⠶⠭⡼⣿⠀⠀⠀⠀⠀⠀⢸⣯⣽⣭⣫⣝⣿⣛⢿⣻⣟⡞⣼⣿⡽⣲⢟⡭⣖⢿⡶⢟⡳⣽⣿⣿⣿⣿⡀⠀⢰⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣻⣽⣾⣿⣯⣟⣿⣞⣽⣿⣷⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣟⣿⣽⡾⣽⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⣼⣫⡝⣞⡹⣟⡾⣹⣻⢧⢻⡟⣽⣿⣷⠶⠶⣤⣤⣤⣤⣀⣀⣀⣀⣀⡀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⢸⠀⠈⠉⠉⠉⠉⠉⠉⢹⣯⢿⡞⣵⣏⠷⣏⡾⡹⣮⢏⣷⣛⣿⣿⣿⣿⣿⣇⠀⣸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⢯⣽⣝⣯⣿⣳⢿⣮⣿⣻⣿⣾⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣹⣽⣿⢽⣺⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣳⣵⣯⢟⣿⣳⣼⣣⡟⣾⣹⣏⢏⣿⣿⠛⠓⠶⠶⠶⢦⣭⣭⣭⣭⣍⣟⣹⣛⣟⣯⠀⠀⠀⠀⠀⠀⣹⣶⠖⡶⠶⡶⠶⠶⣦⣾⣯⣗⢿⡮⣜⣽⡚⢷⡻⣬⣛⣶⢹⣿⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣟⡿⣷⣽⡿⣞⡯⣟⣿⣿⣿⣯⣿⣿⣿⣽⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣿⣽⡛⣧⢿⡡⢿⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣻⢲⣿⣺⣽⠺⣵⢧⡳⣥⢭⣻⢎⣿⣿⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠉⠛⡷⣶⠶⢶⠛⠛⠛⠛⠚⠓⠛⠛⠷⠷⢾⣷⡮⢷⣻⣟⣦⢻⣥⠿⣵⣫⣎⣿⣿⣿⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿⣿⣿⡿⣿⣏⢿⣾⣟⣾⡿⣟⣿⣿⣿⣿⣿⣿⣿⣿⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣳⣙⣻⣧⡟⡗⣏⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⡷⣧⢻⣼⢵⡻⢞⡹⡷⡟⡷⣮⢚⣿⣏⣟⣷⣒⣶⡶⠶⡶⡤⢴⢴⣦⣤⣤⣤⣄⣠⣇⠧⠐⢻⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀⢸⣟⣯⢯⢶⣯⢎⢷⡫⣝⡧⣚⣵⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⡝⣯⣟⡷⣟⣧⣿⣽⣿⣿⣿⣿⣿⣿⢿⡃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣛⡷⣻⣼⢻⡵⣎⢿⢿⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡥⡟⣮⢳⢝⣻⣝⡳⡽⡼⡻⣽⣿⡏⠉⠉⠉⠉⠉⠙⠛⠛⠛⠛⠒⠳⠾⠶⠶⠿⡏⡓⠀⢸⣽⣭⣭⣿⣹⣛⣛⣛⣟⣲⣾⣟⡶⢯⣾⣭⣿⣯⢞⡾⣵⡽⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⡏⡏⣽⣿⣿⣷⣻⡿⣿⣿⣿⣿⣿⡿⣟⣿⣭⣿⢾⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⢮⣝⣻⢻⣣⣳⢞⣫⢳⡭⣿⣿⣿⣿⣿⣿⣿⢿⣿⡿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣻⡝⢮⡻⣏⢯⡾⡧⣟⣛⡷⡼⢿⣷⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣱⠀⢺⠀⠀⠀⠀⠀⠈⠈⠉⠉⠉⠹⣯⡜⣿⣻⣳⡭⡿⢫⢮⡽⣟⢾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⢀⣀⣦⣾⣿⠁⣿⠀⢿⣿⣿⣷⣿⣿⣿⣟⢯⣯⣽⣳⣏⡿⣷⣻⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⡷⢞⣭⢷⣛⣮⣻⡥⣯⢓⣿⣿⣿⣿⣿⣿⣿⣿⢿⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⡝⣭⣓⣯⢎⡿⣵⡽⣮⣝⡸⣽⣿⠿⠿⠯⣯⣯⣹⣟⣛⣳⣒⣲⠶⡶⢶⢶⢶⣟⠢⠀⢹⣤⣤⣤⣤⣤⣤⣀⣀⣀⣀⣀⣿⢾⡟⣽⡳⣷⣛⡭⢏⠷⣚⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⣤⣤⣴⣶⣿⣿⣿⣿⣿⣿⣶⢘⣧⣼⣿⣿⣿⣿⣷⣻⢾⣯⣾⢧⣟⣾⣽⡿⣷⣻⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣻⣿⣿⢮⣽⢎⡗⣼⣲⢟⠮⣳⢹⣿⣿⣿⣿⣿⣿⣾⣿⣟⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⣿⣿⣾⣱⢮⡶⡝⣹⣷⢏⣲⠞⣧⣻⣿⡗⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠙⠛⠛⠛⠻⣿⠄⠀⣾⠷⠾⠶⠮⣴⣮⣶⣭⣮⣹⣽⣿⢯⢾⣝⣟⡶⣽⣹⣯⢝⣣⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠶⣼⣿⢷⣺⣽⣿⣿⣿⣺⢗⣯⣿⣽⣻⢿⣿⣷⣯⢿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣵⣿⣿⣞⢮⠿⡬⡗⣮⢻⡝⣮⢿⣿⣿⣿⣿⣿⣿⣿⢯⣿⣿⣿⣿⣯⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⣿⣧⢯⣳⠭⡽⣝⣫⣗⢧⣯⢻⣟⣿⣷⠤⣤⣤⣤⣤⣀⣀⣀⣀⣀⣀⠀⠀⠀⠀⣿⢟⠁⣽⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⡿⢶⡫⢶⣯⣗⣻⡱⣞⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡺⣿⣿⠿⠿⠿⠿⠛⠛⠉⠁⠀⠀⠀⣿⣻⣭⢷⣞⣯⢿⣿⣿⣾⢟⡷⣏⣿⣿⣧⣿⣽⣻⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣗⣷⢿⣿⣟⣺⡻⣵⡿⢞⡯⣞⠻⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⢿⣻⣽⣿⣟⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣫⢎⣽⢳⣹⢱⣮⡻⣼⣋⡞⣽⣿⠾⠶⠵⢶⣧⣖⣩⣍⣭⣍⣏⣛⢻⣛⢻⣧⡂⠀⢽⠶⠶⡤⣤⢤⣤⣤⣤⣤⣀⣀⣸⣿⢷⡹⢷⡶⣮⣱⠿⣥⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣿⣿⡍⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣹⣽⣿⢻⣯⣿⢿⣿⣿⣿⣽⣯⢻⣿⣯⣿⣞⣿⣾⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣽⣿⣿⣿⣿⢶⣹⣙⣮⡏⣞⣳⢿⣸⣿⣿⣿⣿⣿⣿⣿⣻⣿⣿⣿⣿⣿⣿⢿⣟⣿⣿⣟⣿⣿⣿⣿⣽⣿⣿⢾⡜⣯⣚⡗⡼⣧⢳⢫⣽⢾⣿⡆⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠛⠛⡧⠥⠀⣼⠛⠻⠶⠷⠶⠷⠶⠭⢦⣥⣾⣿⣿⢧⣻⡗⣾⣱⣞⣽⡞⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡾⣝⣿⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣯⢯⣟⣿⣷⣯⣯⡿⣿⣿⣞⣿⣟⣧⣿⣽⣟⣷⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡿⣿⣽⣻⣿⡿⣧⢏⡶⢳⣞⢵⡳⣳⢻⣿⣿⣿⣿⣿⣿⣿⣿⢿⣿⣾⣿⣯⣿⣿⣿⢿⣻⣿⣿⣿⣿⣿⣽⣿⣿⣿⢜⡳⢎⡿⣷⡭⡿⣱⡏⣯⣿⣟⢖⡶⠶⡶⢤⣤⣤⣤⣀⣀⣀⣀⣀⣀⡗⠃⠀⢹⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣯⠳⣍⡷⢗⡮⣶⣝⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⣷⣻⣽⣷⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⣿⢿⣿⡽⣯⣛⣾⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣹⣯⢞⣽⠾⣿⣣⣏⢿⣱⢎⡷⣹⢮⣽⣿⣿⣿⣿⡿⣿⣿⣿⣿⣻⣿⣽⣿⣿⣷⣿⣿⢿⣿⣽⣿⣿⣿⣿⣿⣿⣿⣷⡫⣟⡼⡵⢟⣻⣴⣹⠶⣿⡏⠛⠛⠚⠓⠺⠶⠷⠶⠵⠧⣾⣭⣋⣽⣗⠃⠀⣾⣟⣻⢟⡛⢻⢲⠲⢶⡲⢶⠶⢶⣾⣷⡟⣳⢞⣫⢾⠼⣞⣼⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⣿⣷⣧⢟⣿⣷⠀⠀⠀⠀⠀⠀⠀⣸⣿⣿⣿⣟⡾⣽⣟⣯⣾⣝⣿⣿⣿⡶⣯⣽⢿⢿⢿⣅⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡟⣧⢯⣟⣟⣺⣏⠷⢎⡷⣇⡯⣳⣹⢧⣾⣿⣿⣿⣿⣿⣿⣿⣿⣟⣿⣿⣿⣿⣿⣾⣟⣿⡿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣹⣶⢹⠯⣝⢏⣾⢷⡻⣿⣇⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣯⠭⠀⣿⠉⠉⠙⠛⠛⠛⠛⠓⠛⠓⠺⠖⠾⣿⡟⡼⣿⣣⢿⡿⣬⣻⣿⣿⣿⣿⣿⣯⣿⣿⣿⣿⣿⣿⣿⡷⣿⣻⣿⢿⣷⡀⠀⠀⠀⠀⢠⣿⣿⣿⠿⢿⣟⡿⢾⣳⣽⣳⣯⢿⣿⣿⣷⣿⢾⣿⣟⣿⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣻⡷⣟⣽⣟⣺⢭⡻⣏⣗⡺⣕⣿⡸⣭⢿⣿⣿⣿⣿⣿⣻⣽⣿⣿⣿⣿⣿⣽⣿⣿⣿⡿⣟⣿⣽⣾⣿⣿⣿⣿⣿⣿⣿⣵⢚⣯⣝⡣⣟⡮⡷⣹⣿⣿⣝⣛⣛⣟⢳⠶⠶⠶⡶⠴⢤⣤⣤⣤⡟⠒⠀⣾⣀⣀⢀⠀⢀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣽⣏⠷⣭⢻⣽⡲⣽⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⣿⣿⣿⣿⡷⣽⣻⠿⣽⣻⣻⣶⣶⣶⣾⣿⣯⣿⣄⣸⣟⣯⣿⣹⣿⣷⣯⡿⣿⣿⣿⣾⣿⣾⣿⣷⣿⣷⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣝⣾⣻⢻⣦⢳⡏⣞⡽⣚⣱⣻⣜⡜⡷⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣿⣽⣿⣿⣿⢿⣟⣿⣿⣿⣿⣾⣿⣮⠷⣜⢮⡽⣹⣯⡳⢳⣿⣧⠀⠀⠉⠉⠉⠙⠛⠛⠛⠛⠒⠶⠾⠾⣧⠃⠀⣿⣭⣭⣏⣿⣛⣏⣟⣛⣟⣛⣶⣲⣶⢾⣿⡞⢯⣽⢷⣺⢷⣻⣿⣿⣿⣿⣿⣷⣿⣿⣿⡿⣿⣿⣿⣿⡽⣞⣹⣯⢷⣿⢿⣾⣹⢿⠾⣽⣿⣻⣽⣿⠿⣷⡾⣽⢯⣟⣷⢿⣹⣿⣟⣿⣿⣟⣿⣻⣿⣿⣿⣦⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣯⢿⡞⣭⢷⣪⠷⣝⢧⣞⡭⢧⡇⣾⢺⡱⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⣿⢿⣿⢿⣾⣿⣿⣯⣿⡿⣿⣽⣿⢿⣽⣿⣿⣿⣝⢣⡟⣻⡥⣗⣟⣳⣿⣧⣤⣤⣀⣀⣀⣀⣀⠀⠀⠀⠀⠀⠀⢀⡯⠦⠀⣿⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠉⠉⠙⣿⣟⣯⣜⢻⡾⣗⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣼⣳⣝⣮⡻⣼⡻⣟⠿⣯⡟⣽⢿⣷⣟⣇⠀⣸⣿⣵⢿⣯⣟⣾⡳⣽⣿⡾⣯⣿⣿⣿⣿⣾⣿⣿⣻⡿⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢈⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣛⣮⣟⣽⣎⡵⣯⢟⡝⡮⢗⣯⢭⠿⣜⣷⣻⣿⣿⣿⣿⣿⣯⣿⣿⣿⣿⣯⣿⣿⣿⣿⡿⣿⣿⣿⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⢻⣼⡓⣎⡷⣟⣮⣿⡟⠒⠷⠷⠿⠼⠽⣭⣯⣽⣍⣻⣙⣟⣻⡧⡡⠀⣿⠶⡶⠶⢶⡴⢤⢤⢤⣤⣤⣤⣤⣤⣤⣿⣿⡳⣜⢯⢳⣛⣽⣿⣿⣷⣿⣿⣾⣿⣿⡿⣿⣿⣿⣿⣿⣿⣶⢯⣶⠻⣵⣻⡽⣏⣷⢻⢮⣽⣿⣾⡽⣿⣟⣮⢷⢿⣳⢿⣾⢽⣳⢾⣿⣳⣿⢿⣿⣿⣟⣿⣿⣿⣽⣟⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣏⣿⡼⣾⢭⡗⣮⠻⡝⡾⣫⢼⣓⢿⡸⢖⣻⣿⣿⣿⣿⣿⢿⣷⣻⣿⣿⣟⣿⣾⣽⣿⣿⣿⣯⣷⣿⣿⣾⣿⣽⣯⣿⣿⣽⣿⣿⡳⡼⣟⣝⡾⣻⣯⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⢙⡷⠄⡀⣿⠛⠛⠓⠓⠿⠾⠧⠷⠶⠶⣵⣾⣴⣣⣺⣷⣻⣜⣏⣏⣧⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣻⢏⣧⢷⣹⢷⣺⢯⣗⡟⣿⣷⣻⡷⣾⣹⣞⣯⢿⣯⢻⣟⣯⣟⣿⣷⣯⢿⣹⣿⣻⣷⣟⡿⣿⣿⣾⢿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⣛⣷⢻⣆⣷⣛⡶⡴⣳⢇⡾⣟⢤⣳⡞⣿⣿⣿⣿⣿⣿⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣿⣻⣿⣿⣼⣿⣼⣿⣿⣿⣿⣿⣿⣿⣗⢧⡻⣤⡳⢧⣧⣿⣗⠶⢦⠤⣤⣤⣀⣀⣀⣀⣀⠀⠀⠀⢸⡇⡆⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⢿⣧⡿⢤⢣⢧⣼⣿⣿⣿⣧⡿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢼⣚⡼⡾⣸⢆⣟⣾⣸⣗⡿⣿⣼⡷⣿⣣⡿⣼⡾⣼⣞⣻⡼⣟⢿⣿⣻⢾⣧⢿⣿⣻⣿⣷⢿⣿⣟⣾⢿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⠽⣿⣜⠾⣎⡷⣛⡧⢿⡳⣝⡳⣇⡟⣿⣿⣿⣿⣿⣿⢿⣷⣿⣿⣿⣿⣟⣿⣿⣿⣿⡿⣿⣿⡽⣿⣯⣿⣷⣿⣽⣿⣯⣿⣿⣯⣇⣷⢛⣾⣙⠶⣿⣟⠛⠛⠓⠳⠾⠶⠭⢶⣭⣭⣍⣛⣻⣻⡷⠄⠀⣿⠲⢖⡶⠶⡶⢦⢤⡤⣤⡤⡤⣤⣤⣤⣤⣼⣿⢏⣯⣛⣎⡿⣿⣿⣾⣿⣿⣿⣾⣟⣿⣿⣷⣿⣿⣿⣿⣿⣿⣟⢮⣵⢻⡽⣝⢮⣳⣧⢯⣽⣻⣿⣿⣝⣳⣽⡳⣟⣳⣿⡾⣽⣾⢯⣷⣿⣯⠿⣟⣾⣿⣿⣿⣻⢿⣿⣾⢿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢺⣿⣯⢝⡾⣟⢮⣓⣯⢻⣓⣯⣺⡵⣫⣝⢲⣿⣿⣿⣷⣿⣿⣿⡿⣟⣿⣾⣿⣯⣿⣷⡿⣿⣿⣽⡿⣿⣿⣾⣻⣷⣿⡿⣿⣷⣿⣿⡶⣎⡟⡴⡻⣿⣽⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⢹⡗⠄⠀⣿⠛⠛⠛⠛⠓⠚⠒⠛⠲⠳⠿⠶⠷⠬⠶⠭⢿⣿⣹⣷⣞⡹⣿⣿⣿⣿⣿⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣻⢮⠷⡽⣞⡧⣷⣫⠾⡶⣽⣻⣷⢯⢷⣝⣿⡹⣗⢿⣿⣳⣏⡿⣞⣷⡾⣿⣻⢽⡾⣿⣷⣿⣿⣟⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢐⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣽⣿⣿⢯⢻⡼⢧⡻⡼⣹⡧⣟⣼⣱⢝⣮⡽⣿⣿⣿⣟⣿⡿⣷⣿⣿⣿⢿⣿⣻⣯⣿⣿⣿⣽⣿⡿⣿⣳⣿⣿⣽⣿⣟⣿⣾⣿⣿⣿⢥⡟⣽⣣⠷⣿⣿⣛⠷⣒⠲⠶⢶⠶⣤⣤⣤⣤⣄⣀⣸⡟⡔⠀⣯⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣷⡻⣷⢟⣿⣿⣿⣽⣿⣿⣯⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣻⢞⣻⢿⣼⢿⣱⢿⡽⣳⠷⣽⣿⣿⠽⣞⣧⣿⣭⣿⣿⣯⣯⢿⣽⣻⣭⢯⡷⣯⣟⣿⣿⢯⣿⡾⣽⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠨⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⣿⣿⡽⣽⣳⣝⡷⣝⡾⣸⡵⣙⣮⢇⡿⣿⣿⣿⣻⣿⣿⣿⣟⣿⣽⣿⣿⣟⣿⣿⣿⣾⣿⡿⣿⣿⢿⣿⣽⣿⣾⣿⣿⣿⣿⣿⣿⡯⢾⣼⣕⡻⣽⣯⠉⠉⠉⠉⠛⠛⠛⠒⠲⠺⠶⠿⠼⢿⡇⠎⠀⣿⣫⣻⣛⣛⣿⣛⣶⢲⢖⡲⢶⡲⢶⠶⢶⡖⣶⣿⣷⡟⣻⣻⣿⣿⣿⣿⣿⣿⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡷⢯⣳⢯⣛⡮⣿⣭⢷⡿⣯⢷⣻⢿⣿⣻⣼⣶⢟⣽⣿⣷⣯⣟⣯⡷⣯⣿⣽⣻⣾⣿⢿⣟⣾⢻⣟⣽⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣿⣿⣿⣳⣳⢭⣳⠻⣜⢷⡳⣞⢟⡶⣫⣝⣿⣿⣿⣿⣿⣻⣽⣿⣯⣿⣿⣿⣿⣿⣷⣿⢿⣿⣿⣿⣿⣿⣿⣽⣿⣯⣿⣿⣿⣷⣿⣿⣟⡳⢮⢿⣵⣿⣿⣄⣀⣀⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠠⡷⠏⠀⣿⠀⠀⠉⠉⠉⠉⠉⠉⠛⠛⠛⠛⠛⠚⠓⠛⠚⠛⢿⣾⣵⣳⢿⣿⣿⣿⣷⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣳⢯⡿⢼⣻⣖⣿⠾⣵⣯⢻⣽⢯⣟⡷⣽⣞⣯⢿⣿⣟⣾⣳⣯⢿⡿⣷⡿⣯⣟⣾⣻⣞⣿⣿⢾⣽⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣼⣿⣿⣿⡿⢧⣯⢷⣯⢻⡭⣿⣱⢻⣞⡹⣷⢮⣿⣿⣿⢿⣿⣿⣿⣿⡿⣿⣽⣿⣿⣽⣾⣿⣿⣯⣿⣾⣿⣿⣽⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣛⢯⣷⢹⣿⣿⣶⣭⣜⣯⣹⣍⣻⣛⡟⢻⢛⡶⢳⣾⣧⡓⠀⣿⣤⣤⣤⣤⣤⣤⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣸⣿⣎⡗⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡳⣏⣿⣫⢷⣛⣞⡿⣳⣏⣿⡽⣾⡝⣿⠾⣽⣾⣻⣾⣿⣽⣯⣿⣻⡿⣽⣻⣽⣞⣯⣟⣿⣻⣞⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⣠⡿⣿⢽⡻⣽⣿⣳⣯⣷⣿⣽⣞⡭⣷⡽⣱⢏⣷⡞⣿⡜⢯⣿⣿⣿⣿⣿⣿⣻⣷⣿⣿⣿⣿⣿⡿⣿⣿⣯⣿⣿⣿⡿⣿⣿⣿⣿⣾⣿⣿⣿⣿⣿⣿⣿⣟⢷⣺⠿⣿⣷⠀⠀⠈⠉⠉⠉⠉⠙⠛⠛⠛⠛⠛⠾⡟⡄⠀⣿⠶⠿⠶⠵⠮⡼⣭⣭⣯⣽⣭⣯⣯⣽⣭⣿⣩⣯⣽⣿⣾⡹⣞⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣽⢾⣽⣫⣟⡾⣿⣳⢯⢯⣽⣻⢾⣷⣻⢯⡷⣽⢯⣿⣾⣯⢷⣻⣿⡷⣟⣿⣾⣞⡾⣯⣷⣿⣻⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⣴⣿⣻⣯⣿⣾⣿⢻⣭⣿⣿⣽⡻⣟⣻⣕⣯⣷⣟⡻⣽⡹⣾⣝⣿⣿⣿⣿⢿⣽⣿⣿⣻⣿⣽⣾⣿⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣛⣷⡻⣿⣿⣤⣤⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠤⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⠷⣭⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣾⣳⣽⣳⣾⢳⣿⣹⡾⣻⢾⣽⣳⣯⢷⣻⡽⣯⢿⣿⣯⢯⣟⣯⣿⣿⡽⣿⣿⣾⡽⣯⡷⣯⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⣼⡟⣮⣿⣿⣿⣿⡿⣟⣟⣳⣏⢷⣿⣾⣿⣿⢻⡟⣷⣫⡽⣽⢳⢾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣿⣿⣿⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⣶⢿⣿⡿⠾⠷⣵⣯⣭⣛⣟⣻⣟⣻⢻⢲⢳⣾⡏⠆⠀⣿⢶⠶⡶⢶⠶⣶⠶⣶⢶⠶⡶⢶⡶⢶⢶⡶⢶⠶⣤⣤⣿⣿⢯⣿⣿⣿⣿⡿⣿⣻⣯⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣟⡾⣗⣻⡼⣿⢖⣯⣟⣿⣻⣞⣯⣞⡿⣽⣽⡷⡿⣿⡿⣿⣽⣻⣾⣿⣿⣽⣿⣿⣳⣟⣿⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⢰⣯⣿⣛⣯⣿⢟⣳⡽⣺⣭⣷⣿⣿⣿⣿⢿⣟⣫⣽⣯⢳⣽⣳⢟⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢯⣿⣷⣟⣿⣿⣿⣿⣿⣿⣿⣯⣿⣿⣿⣿⣿⣿⣿⣿⣝⡾⣿⣿⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠛⠛⠛⠛⣯⠃⠀⣿⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠉⣿⣻⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⣹⢯⡷⣽⣻⡟⣿⣞⡷⣯⢷⣾⢞⡿⣽⢾⣝⣿⣽⣿⣳⣯⡷⣿⣿⡿⣾⢿⣿⣿⣿⢿⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⣾⡿⢧⡿⣼⣫⣞⣧⠿⣽⣿⣿⣿⣿⣟⣯⢷⣻⣲⢷⡫⣯⢧⡟⣮⣿⣿⣿⣿⢿⣿⣯⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣻⣿⣿⣿⣿⢿⣿⣿⣿⣿⣿⢿⣿⣿⡾⣽⢿⣿⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣏⡽⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣿⢷⣿⣿⣿⣿⣿⡿⣟⣿⣿⣿⣿⣿⣿⣟⣿⣿⣿⣿⡷⢯⡿⣽⣳⢯⣟⣿⢞⡿⣽⣻⣞⣿⢽⣯⢿⣾⣽⢾⣿⣽⡻⣽⣻⣿⡿⣽⣿⣿⣳⢿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⣿⣝⣿⡳⢯⡷⣞⡾⣿⣿⣿⣿⡿⢯⡷⢮⢟⣧⡻⣏⡷⣭⢻⢾⣱⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣿⣿⣿⣿⣿⣿⣿⣿⣟⣿⣿⣿⣿⣿⣟⣵⣻⣿⣾⣿⣻⣯⣟⣻⣽⣛⣿⣓⡶⡶⣶⣴⣏⡓⠀⣿⣤⣄⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣠⣤⣿⣯⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣽⢷⡯⣟⣾⡽⣯⣟⣽⢯⣾⣽⣫⢷⣟⡾⣽⣻⣿⣯⣟⡿⣷⣿⣿⡷⣿⣿⣻⣾⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⢹⣿⣷⣽⣳⢯⣽⣿⣿⢳⣿⣟⡿⣟⡹⣯⢞⣧⡽⢯⣵⢻⣝⡮⢷⣾⣿⣿⣷⣿⡽⠛⠉⠁⠀⠀⠀⠈⢹⣿⣿⣾⣿⣿⣷⣿⣿⣯⣿⣷⣿⣿⣾⣿⣿⣿⣿⣿⣟⣿⣿⣿⣞⣿⣿⠀⠀⠀⠀⠀⠈⠉⠉⠉⠙⠛⠓⠻⠿⣧⡓⠀⣿⣾⣽⣫⣯⣽⣏⣻⣝⣫⣟⣽⣫⣝⣯⣽⣯⣿⣭⣯⡷⠷⠿⠿⣿⣿⣿⣿⣿⣿⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣽⣳⡿⣽⣻⡼⣿⡽⣻⢾⣻⢾⣽⢯⣟⣾⣽⣳⢿⣿⣟⡿⣽⡿⣿⡿⣟⣿⣿⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⢻⣿⣾⡻⣯⣿⣿⣼⣻⢽⢯⡷⣫⣷⣋⣿⢮⡽⢯⣽⢻⣞⢽⣯⣽⡿⣿⠛⠋⠀⠀⢰⠂⠀⠀⠀⠀⠐⣿⣿⣿⣿⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣽⣷⣿⣿⣿⣿⣿⣿⡾⣳⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⠬⡁⣿⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡿⣽⣞⣷⣻⣽⣳⣿⢿⣽⣟⡾⣟⡿⣞⣷⣻⣟⣿⣽⣿⡽⣻⣿⣿⣿⣽⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠹⣿⣿⣿⡻⣷⣽⣫⠿⣽⡽⢧⣟⣾⣚⣯⡽⣯⢷⣿⡾⠛⢁⡤⠖⠛⠓⠲⠦⢤⣸⠀⠀⠀⠀⠀⢰⣿⣿⣿⣯⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣗⣲⣲⠶⣦⣤⣤⣤⣤⣤⣤⣤⣸⡗⣋⠀⣿⣀⣀⣀⣀⣀⣀⢀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⢀⣀⣀⣀⣀⣀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣽⣻⣾⣽⢯⣷⣻⢿⣺⣾⣟⣯⣽⣟⡷⣟⣾⣿⣻⣞⣿⣽⣿⣷⣿⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠘⢿⣿⣹⣟⢧⣻⡝⣿⡼⣯⢷⣯⣞⣾⡹⣯⡾⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⡇⠀⢰⠋⠳⣜⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣿⣿⣿⡿⣿⣿⣿⣿⣿⡾⣿⠉⠙⠓⠛⠿⠷⠾⠼⠶⠿⠾⢽⣾⣿⡿⡅⠀⣿⣝⣟⣿⣹⣯⣻⣯⣽⣻⣝⣏⣿⣹⣟⣟⣟⣯⣻⣭⣿⣭⣿⣽⣻⡼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⢯⣽⣾⣻⢾⣿⣻⣟⣧⣿⡽⣯⣽⣟⣯⣿⣿⣽⣾⣿⣿⣽⣿⡾⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠈⠻⣿⣿⣹⣯⣽⣷⣻⣽⣿⣽⢮⣷⣿⣽⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣄⠸⡤⠒⠛⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡷⠅⠀⣿⠀⠀⠀⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠁⠀⠀⠀⠀⠀⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⡿⣾⣿⣻⣿⣿⣳⣟⣿⣻⣿⣿⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⢀⢀⢀⢀⠀⠀⠀⠀⠀⠀⠀<br>"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣷⣿⣿⣽⣿⣿⣿⣿⣟⣿⣷⡿⣷⠦⢤⣤⣤⣤⡤⠤⠶⠶⠖⠛⠛⠉⠉⠲⢤⣄⣤⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣶⣶⣶⣶⣶⣤⣤⣤⡤⣤⢼⡿⢿⠀⡿⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠋⠙⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀<br>"
"   ⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠈⠁⠉⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀            <br>"
"⠀⠀⠀⢀⣠⣤⠀⠀⠀⠀⣤⣄⡀⠀⠀ ⠀<br>"
"⠀⢀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡄⠀<br>"
"⠀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀<br>"
"⢸⣿⣿⣿⠁⠀⠉⣿⣿⠋⠀⠈⢻⣿⣿⡇<br>"
"⢸⣿⣿⣿⣤⣠⣴⣿⣿⣧⣄⣤⣿⣿⣿⣿<br>"
"⠸⢿⣿⣿⣙⠛⠿⠿⠿⠿⠟⢛⣿⣿⣿⠟<br>"
"⠀⠀⠉⠙⠁⠀⠀⠀⠀⠀⠀⠈⠛⠉⠀⠀<br>"                     

"<p><a href=>made  by julian.b if you liked this prank hit me  up on discord@ snowball9734 or"
                     " even support me my sending any amount to my cash app @ bulian6fob thank you."
                       "sorry if you did not like this prank </a></p></body></html>";

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("RickRoller started...");

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  
  Serial.println("Setting SSID to '" + never_gonna[0] + "'.");
  WiFi.softAP(never_gonna[0].c_str());

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // reply to all requests with same HTML
  webServer.onNotFound([]() {
    webServer.send(200, "text/html; charset=utf-8", responseHTML);
  });
  webServer.begin();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
  
  static unsigned long last_update_time = 0;
  if (millis() - last_update_time > update_interval) {
    static int current_line = 0;
    current_line = (current_line + 1) % (sizeof(never_gonna) / sizeof(String));
    Serial.println("Setting SSID to '" + never_gonna[current_line] + "'.");
    WiFi.softAP(never_gonna[current_line].c_str());
    last_update_time = millis();
  }
  delay(1000);
}