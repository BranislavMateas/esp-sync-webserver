const char mojeJs[] PROGMEM = R"=====(
var switchValue;
            window.onload = function () {
                switchValue = localStorage.getItem("switchValue") == "true" ? true : localStorage.getItem("switch") == "false" ? false : false;
                document.getElementById("teplota").innerText = switchValue ? "°F" : "°C";
            };

            var toggleSwitch = document.getElementById("toggleSwitch");
            toggleSwitch.addEventListener("change", function () {
                if (this.checked) {
                  document.getElementById("jednotka").innerText = "°F";
                  switchValue = true;
                    localStorage.setItem("switchValue", "true");
                    dajData();
                } else {
                  document.getElementById("jednotka").innerText = "°C";
                   switchValue = false;
                    localStorage.setItem("switchValue", "false");
                    dajData();
                }
            });

            function dajData() {
                var fetchString = "data";
                console.log(switchValue);
                if (switchValue) {
                    fetchString += "?units=f";
                }

                fetch(fetchString)
                    .then((odpoved) => odpoved.json())
                    .then((obsah) => {
                        document.getElementById("teplota").innerHTML = obsah["teplota"];
                        document.getElementById("vlhkost").innerHTML = obsah["vlhkost"];
                    });
            }

            let countdown = 20;
            function handleRefreshClick() {
                dajData();
                // document.getElementById("refresh-button").disabled = true;
                document.getElementById("refresh-button").innerHTML = `<i class="fas fa-sync"></i> ${countdown}`;
                const countdownInterval = setInterval(function () {
                    countdown--;
                    document.getElementById("refresh-button").innerHTML = `<i class="fas fa-sync"></i> ${countdown}`;
                    if (countdown === 0) {
                        clearInterval(countdownInterval);
                        document.getElementById("refresh-button").innerHTML = '<i class="fas fa-sync"></i>';
                        countdown = 20;
                    }
                }, 1000);
            }

            handleRefreshClick();
            setInterval(function () {
                handleRefreshClick();
            }, 20000);
)=====";
