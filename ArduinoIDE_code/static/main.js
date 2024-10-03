const status_div = document.getElementById("status");
const led = document.getElementById("led");


function fetch_messages() {
    console.log("Fetching messages from server");
    const request = new Request("/last_message", {method: "GET"});
    fetch(request).then(
        response => response.text()
    ).then(text => {
        status_div.innerText = text;
    });
}



function led_state_change(){
    const request = new Request("/led_state",{method: "POST"});
    fetch(request).then(
        response => response.text()
    );
}


led.addEventListener("click",led_state_change)
//setInterval(fetch_messages, 500);
 