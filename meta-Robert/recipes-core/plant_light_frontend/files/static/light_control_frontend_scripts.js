const checkbox = document.getElementById('lightSwitch');
const status = document.getElementById('status');

checkbox.addEventListener('change', () => {
    const state = checkbox.checked ? 'on' : 'off';
    status.textContent = `Light is ${state.toUpperCase()}`;


    console.log(`Light is ${state.toUpperCase()}`)

    // send request to Bottle backend
    fetch(`/light/${state}`)
        .then(response => response.text())
        .then(data => console.log(data))
        .catch(err => console.error(err));
});


function onDateTimeSubmit(e) {
    e.preventDefault();

    console.log("Form submit handler called!");

    const date = document.getElementById("date").value;
    const time = document.getElementById("time").value;
    const on_or_off = document.getElementById("scheduleOnOrOffSwitch").checked; 

    console.log("Collected values:", date, time);

    fetch("/submit_datetime", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ date: date, time: time, on_or_off: on_or_off })
    })
    .then(r => r.json())
    .then(data => {
        console.log("Server returned:", data);

        document.getElementById("response").innerText =
            "Server response: " + data.message;
    })
    .catch(err => {
        console.error("Fetch error:", err);
    });
}

// Attach the event listener AFTER DOM loads
document.addEventListener("DOMContentLoaded", function () {
    console.log("DOM loaded, attaching submit handler");
    document.getElementById("dtForm").addEventListener("submit", onDateTimeSubmit);
});


const slider = document.getElementById('mySlider');

slider.addEventListener('input', () => {

    // Send the value to backend
    fetch('/update_slider', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify({value: slider.value})
    })
    .then(res => res.json())
    .then(data => console.log('Server says:', data.message))
    .catch(err => console.error(err));
});


async function server_data_cyclic_update() {
    const res = await fetch("/server_data_cyclic_update");
    const data = await res.json(); 
    console.log(data); 

    if (typeof data.temperature !== "number" ||
        typeof data.relative_humidity !== "number") {
        throw new Error("Invalid data format");
    }
    else {
        // update table cells
        document.getElementById("env_table_temp").textContent =
            data.temperature.toFixed(2);

        document.getElementById("env_table_rel_hum").textContent =
            data.relative_humidity.toFixed(2);

    }


}
//setInterval(server_data_cyclic_update, 1000); 