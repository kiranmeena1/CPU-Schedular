document.getElementById('addProcess').addEventListener('click', () => {
    const processDiv = document.createElement('div');
    processDiv.className = 'process';
    processDiv.innerHTML = `
        <input name="pid" placeholder="PID">
        <input name="arrivalTime" placeholder="Arrival Time">
        <input name="burstTime" placeholder="Burst Time">
        <input name="priority" placeholder="Priority">
    `;
    document.getElementById('processes').appendChild(processDiv);
});

document.getElementById('submit').addEventListener('click', async () => {
    const processes = [];
    document.querySelectorAll('.process').forEach(process => {
        processes.push({
            pid: process.querySelector('input[name="pid"]').value,
            arrivalTime: process.querySelector('input[name="arrivalTime"]').value,
            burstTime: process.querySelector('input[name="burstTime"]').value,
            priority: process.querySelector('input[name="priority"]').value
        });
    });

    const choice = document.getElementById('algorithm').value;
    const quantum = document.getElementById('quantum').value;

    const data = { processes, choice: parseInt(choice), quantum: parseInt(quantum) };
    
    try {
        const response = await fetch('http://localhost:8080/schedule', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(data)
        });

        const result = await response.json();
        displayResults(result.processes);
    } catch (error) {
        console.error('Error:', error);
    }
});

function displayResults(results) {
    const resultsDiv = document.getElementById('results');
    resultsDiv.innerHTML = '';

    results.forEach(result => {
        const resultDiv = document.createElement('div');
        resultDiv.className = 'result';
        resultDiv.innerHTML = `
            <p>PID: ${result.pid}</p>
            <p>Arrival Time: ${result.arrivalTime}</p>
            <p>Burst Time: ${result.burstTime}</p>
            <p>Waiting Time: ${result.waitingTime}</p>
            <p>Completion Time: ${result.completionTime}</p>
            <p>Turnaround Time: ${result.turnaroundTime}</p>
        `;
        resultsDiv.appendChild(resultDiv);
    });
}
