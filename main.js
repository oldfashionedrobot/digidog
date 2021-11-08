(function () {
  const ipInput = document.getElementById('ip-input');

  console.log(ipInput);

  document.getElementById('wind-on').addEventListener('click', (e) => {
    e.preventDefault();
    console.log('clicked wind ON - sending to: ' + ipInput.value);

    sendRequest('windon');
  });

  document.getElementById('wind-off').addEventListener('click', (e) => {
    e.preventDefault();
    console.log('clicked wind OFF - sending to: ' + ipInput.value);

    sendRequest('windoff');
  });

  document.getElementById('smell-on').addEventListener('click', (e) => {
    e.preventDefault();
    console.log('clicked olfaction ON - sending to: ' + ipInput.value);

    sendRequest('smellon');
  });

  document.getElementById('smell-off').addEventListener('click', (e) => {
    e.preventDefault();
    console.log('clicked olfaction OFF - sending to: ' + ipInput.value);

    sendRequest('smelloff');
  });

  function sendRequest(endpoint) {
    let url = `http://${ipInput.value}/${endpoint}`;

    fetch(url, {
      method: 'GET',
      mode: 'no-cors',
      headers: {
        'Content-Type': 'application/json',
      },
    })
      .then((resp) => resp.json())
      .then((json) => {
        console.log(json);
      });
  }
})();
