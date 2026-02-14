% rebase('base.tpl')
<div class="e-container">
    <h1>Szia Ági 🌱🎈</h1>
    
    <label class="switch">
    <input type="checkbox" id="lightSwitch">
    <span class="slider"></span>
    </label>

    <p id="status">Light is OFF</p>

    <h2>Schedule Light turn On or Off</h2>
    <form id="dtForm">
        <label>Date:</label>
        <input type="date" id="date" required><br><br>

        <label>Time:</label>
        <input type="time" id="time" required><br><br>

        <label>On/Off:</label>
        <input type="checkbox" id="scheduleOnOrOffSwitch">
        <button type="submit">Send</button>
    </form>

    <p id="response"></p>

    <input type="range" id="mySlider" min="0" max="100" value="0">


    <table id="envDataTable" cellpadding="6">
    <thead>
        <tr>
        <th>Temperature [C]</th>
        <th>Relative Humidity [%]</th>
        </tr>
    </thead>
    <tbody>
        <tr>
        <td id="env_table_temp">-</td>
        <td id="env_table_rel_hum">-</td>
        </tr>
    </tbody>
    </table>

    <button id="fetchBtn">Fetch Data</button>
    <div id="chart" style="width: 800px; height: 400px;"></div>

    <form action="/submit" method="post">
        <input type="text" name="msg" placeholder="Enter something">
        <button type="submit">Send</button>
    </form>
    
    % if message:
        <h3>You sent: {{message}}</h3>
    % end



</div>


