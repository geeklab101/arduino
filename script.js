// Simulate initial values
var trolley_status = 0;
var scanning_status = 0;
var selected_station = 0;
var moving_status = 0;
var obstacle_status = 0;

// Function to update UI based on received variables
// Function to update UI based on received variables
function updateUI(trolly_status, scanning_status, selected_station, moving_status, obstacle_status) {
    // Update Trolley Status
    var trolleyStatusText = trolly_status === 0 ? 'Trolley is Empty' : 'Trolley is Full';
    console.log('trolley Status text:', trolleyStatusText);
    console.log('trolley Status:', trolly_status);
    
    // Update Scanning Status
    var scanningStatusText = scanning_status === 0 ? 'Scanning: Off' : 'Scanning: On';
    document.getElementById('scanningStatus').innerText = scanningStatusText;

    // Show/hide selected station buttons
    document.getElementById('section1Button').style.display = selected_station === 1 ? 'inline-block' : 'none';
    document.getElementById('section2Button').style.display = selected_station === 2 ? 'inline-block' : 'none';
    document.getElementById('section3Button').style.display = selected_station === 3 ? 'inline-block' : 'none';

    // Update Moving Status
    var movingStatusText = moving_status === 0 ? 'Trolley is Moving' : 'Trolley is Steady';
    document.getElementById('movingStatus').innerText = movingStatusText;

    // Update Obstacle Status
    var obstacleStatusText = obstacle_status === 0 ? 'Path is Clear' : 'Obstacle Detected!';
    document.getElementById('obstacleStatus').innerText = obstacleStatusText;

    // Highlight the selected station button
    highlightSelectedStation();
}


// Function to highlight the selected station
function highlightSelectedStation() {
    // Highlight the selected station button
    document.getElementById('section1Button').classList.remove('highlight');
    document.getElementById('section2Button').classList.remove('highlight');
    document.getElementById('section3Button').classList.remove('highlight');
    if (selected_station > 0 && selected_station < 4) {
        document.getElementById('section' + selected_station + 'Button').classList.add('highlight');
    }
}

// Function to toggle scanning
function toggleScanning() {
    scanning_status = scanning_status === 0 ? 1 : 0;
    updateUI();
}

// Function to handle section selection
function selectSection(sectionNumber) {
    selected_station = sectionNumber;
    highlightSelectedStation();
    // Add logic to handle the selected section, e.g., communicate with Arduino
    showStationSection(); // Assuming this function shows the station section
}

// Function to periodically update variables from jsonapi.php
function updateVariables() {
    // Example AJAX request using jQuery
    $.ajax({
        url: './api/datagetting.php',
        type: 'GET',
        dataType: 'json',
        success: function (data) {
            // Access data retrieved from PHP
            trolley_status = data.trolly_status;
            scanning_status = data.scanning_status;
            selected_station = data.selected_station;
            moving_status = data.moving_status;
            obstacle_status = data.obstacle_status;

            // Update UI with new values
            updateUI(trolley_status, scanning_status, selected_station, moving_status, obstacle_status);

            // Call the function again after 1000ms (1 second)
            setTimeout(updateVariables, 1000);
        },
        error: function (error) {
            console.error('Error fetching data:', error);
        }
    });
}

// Initial call to start updating variables
updateVariables();


// // Function to periodically update variables
// function updateVariables() {
//   // Simulate fetching data from the server
//   trolleyStatus = Math.round(Math.random()); // 0 or 1
//   scanningStatus = Math.round(Math.random()); // 0 or 1
//   selectedStation = Math.floor(Math.random() * 3) + 1; // Random number between 1 and 3
//   movingStatus = Math.round(Math.random()); // 0 or 1
//   obstacleStatus = Math.round(Math.random()); // 0 or 1

//   // Update UI with new values
//   updateUI();

//   // Call the function again after 1000ms (1 second)
//   setTimeout(updateVariables, 1000);
// }

// // Initial call to start updating variables
// updateVariables();
