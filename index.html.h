const char mojeHtml[] PROGMEM = R"=====(<!DOCTYPE html>
<html lang="en">
    <head>
        <title>ESP8266 DHT Web Server</title>
        <meta charset="utf-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no" />
        <link
            href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.1/dist/css/bootstrap.min.css"
            rel="stylesheet"
            integrity="sha384-iYQeCzEYFbKjA/T2uDLTpkwGzCiq6soy8tYaI1GyVh/UjpbCx/TYkiZhlZB6+fzT"
            crossorigin="anonymous" />
        <link
            rel="stylesheet"
            href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.1/css/all.min.css"
            integrity="sha512-+4zCK9k+qNFUR5X+cKL9EIR+ZOhtIloNl9GIKS57V1MyNsYpYcUrUeQc9vNfzsWfV28IaLL3i96P9sdNyeRssA=="
            crossorigin="anonymous" />
        <link rel="stylesheet" href="styles.css" />
    </head>
    <body>
        <div style="background-color: #1f2630; text-align: center">
            <h1 style="color: white; padding-top: 1%">ESP8266 DHT Web Server</h1>
            <h3 style="color: white; padding-bottom: 1%">{IP_ADDRESS}</h3>
        </div>
        <div style="text-align: center; margin-top: 2%" class="container shadow col-lg-4 col-md-6 col-xs-12 col-sm-12 px-3">
            <h3 id="nadpis" style="padding-bottom: 6%; padding-top: 8%">Branislav Mateas</h3>
            <h5 style="margin-bottom: 3%">
                <img src="https://cdn-icons-png.flaticon.com/512/5105/5105265.png" style="width: 7.5%; height: auto" />
                Teplota
                <span style="margin-left: 1%; font-size: 1.2em; color: rgb(44, 213, 255)" id="teplota">{TEPLOTA}</span>
                <span id="jednotka">°C</span>
            </h5>
            <h5 style="padding-bottom: 10%">
                <img src="https://cdn-icons-png.flaticon.com/512/1779/1779817.png" style="margin-bottom: 1%; width: 7.5%; height: auto" />
                Vlhkosť
                <span style="margin-left: 1%; font-size: 1.2em; color: rgb(44, 213, 255)" id="vlhkost">{VLHKOST}</span>
                %
            </h5>

            <label class="switch">
                <input type="checkbox" id="toggleSwitch" />
                <span class="slider round"></span>
            </label>
            <div class="pb-4">
                <label class="form-check-label" for="flexSwitchCheckDefault">°C - °F</label>
            </div>
        </div>

        <div class="container d-flex justify-content-center align-items-center my-2">
            <button type="button" id="refresh-button" class="btn btn-info" onclick="dajData()">
                <i class="fas fa-sync"></i>
            </button>
        </div>
        <script src="script.js"></script>
    </body>
</html>
)=====";
