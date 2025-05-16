#pragma once

#define BASE_HTML R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>GateBnB Access</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
        body {
            font-family: sans-serif;
            background-color: #f8f9fa;
            margin: 0;
            padding: 0;
        }
        .navbar {
            display: flex;
            justify-content: space-between;
            align-items: center;
            background-color: #343a40;
            color: white;
            padding: 0.8rem 1.5rem;
        }
        .navbar .logo {
            font-size: 1.2rem;
            font-weight: bold;
        }
        .navbar a {
            color: #ffffff;
            text-decoration: none;
            font-size: 0.95rem;
        }
        .container {
            background: white;
            border-radius: 8px;
            padding: 2rem;
            max-width: 400px;
            margin: 2rem auto;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
            text-align: center;
        }
        button {
            font-size: 1.2rem;
            padding: 1rem 2rem;
            margin: 1rem;
            cursor: pointer;
            border: none;
            border-radius: 6px;
        }
        .open-btn {
            background-color: #28a745;
            color: white;
        }
        .close-btn {
            background-color: #dc3545;
            color: white;
        }
        .message {
            font-size: 1.1rem;
            margin-bottom: 2rem;
        }
    </style>
</head>
)rawliteral"

#define GATE_HTML R"rawliteral(
<body>
    <div class="navbar">
        <div class="logo"><img src="https://lvido.tech/img/logo-lvido-tech-light.png" alt="LVido Tecnologia"></div>
    </div>
    <div class="container">
      <div class="message">
        Bem Vindo - Chalé ao Pé da Pedra
      </div>
      <form method="post" action="open">
      <button class="open-btn" name="action" value="open">Abrir</button>
      <form method="post" action="close">
      <button class="close-btn" name="action" value="close">Fechar</button>
      </form>
      </div>
</body>
</html>
)rawliteral"

#define INDEX_HTML R"rawliteral(
    <div class="navbar">
        <div class="logo"><img src="https://lvido.tech/img/logo-lvido-tech-light.png" alt="LVido Tecnologia"></div>
    </div>
    <div class="container">
      <div class="message">
        Bem Vindo! Você está conectado ao Portão
      </div>
      <div>
      Clique no link com token enviado no Whatsapp.
      </div>
</body>
</html>
)rawliteral"

const char index_html[] PROGMEM = BASE_HTML INDEX_HTML;
const char gate_html[] PROGMEM = BASE_HTML GATE_HTML;
