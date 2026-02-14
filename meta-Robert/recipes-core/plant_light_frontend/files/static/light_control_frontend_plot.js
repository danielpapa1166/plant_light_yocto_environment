// Dummy timestamps (Unix seconds)
const timestamps = [
    1704892979,
    1704893039,
    1704893099,
    1704893159,
    1704893219
];

// Dummy float values
const value1 = [23.6, 23.61, 23.63, 23.62, 23.64];
const value2 = [43.48, 43.75, 43.71, 43.80, 43.79];

// Data array for uPlot
const data = [
    timestamps,
    value1,
    value2
];

// uPlot options
const options = {
    title: "Sensor Values",
    width: 800,
    height: 400,
    scales: {
        x: {
            time: true
        }
    },
    series: [
        {}, // x-axis (timestamps)
        { label: "Value 1", stroke: "blue" },
        { label: "Value 2", stroke: "red" }
    ],
};

// Initialize the chart
const chart = new uPlot(options, data, document.getElementById("chart"));
fetchData();

// Helper to convert timestamp string to Unix seconds
function parseTimestamp(tsString) {
  return Date.parse(tsString) / 1000;
}

// Fetch data from Bottle backend
async function fetchData() {
  try {
    const resp = await fetch("/fetch_sensor_data"); // adjust endpoint
    if (!resp.ok) throw new Error("Network response was not OK");

    const json = await resp.json();

    // Convert timestamps to Unix seconds
    const ts = json.timestamps.map(parseTimestamp);
    const temperature = json.temperature;
    const relative_humidity = json.relative_humidity;

    // Update chart
    chart.setData([ts, temperature, relative_humidity]);
  } catch (err) {
    console.error("Fetch error:", err);
  }
}

// Button callback
document.getElementById("fetchBtn").addEventListener("click", fetchData);