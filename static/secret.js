const solveBtn = document.getElementById('solveBtn');
const solveDotProductBtn = document.getElementById('dotProductSolve');
const solveCrossProductBtn = document.getElementById('crossProductSolve');
const diff_form = document.getElementById('dydx');
const multForm = document.getElementById('mult');

function handle_arithmetic() {
    const res = document.getElementById('result');
    const errorLabel = document.getElementById('errorLbl');
    const expression = document.getElementById('expression').value;
    console.log(expression);
    const hdrs = new Headers();
    const params = "op=arithmetic&expression=" + expression;
    const method = "POST";
    const http = new XMLHttpRequest();
    const url = "http://127.0.0.1/secret"
    http.open(method, url, true);
    http.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded')
    http.send(params);

    //On data retrieved listener
    http.onreadystatechange = function () {
        if (this.status === 200) {
            res.innerHTML = http.responseText;
            multForm.reset();
        } else {
            alert("something went really wrong, mr someone")
        }
    }
}

function handle_vector_cross_product() {
    const res = document.getElementById('crossProductSolve');
    const vec1 = document.getElementById('crossVec1').value.toString();
    const vec2 = document.getElementById('crossVec2').value.toString();

    const data = "op=cross&vec1=" + vec1 + "&vec2=" + vec2;
    const method = "POST";
    const http = new XMLHttpRequest();
    const url = "http://127.0.0.1/secret"
    http.open(method, url);
    http.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded')
    http.send(data);
    http.onreadystatechange = function () {
        if (this.status === 200) {
            res.className = "success";
            res.innerHTML = http.responseText
        }
    }
}


function handle_vector_dot_product() {
    const res = document.getElementById('dotProductResult');
    const vec1 = document.getElementById('vec1').value.toString();
    const vec2 = document.getElementById('vec2').value.toString();

    const data = "op=dot&vec1=" + vec1 + "&vec2=" + vec2;
    const method = "POST";
    const http = new XMLHttpRequest();
    const url = "http://127.0.0.1/secret"
    http.open(method, url);
    http.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded')
    http.send(data);
    http.onreadystatechange = function () {
        if (this.status === 200) {
            res.className = "success";
            res.innerHTML = http.responseText
        }
    }
}


function handle_differentiation() {
    const res = document.getElementById('diff-res');
    var func = document.getElementById('func');
    const at = document.getElementById('dydx-at');

    var fn = new String(func.value)
    fn.replace("%20", "\t");
    var headers = new Headers();
    const data = "op=diff&func=" + fn + "&at=" + at.value;
    const method = "POST";
    const http = new XMLHttpRequest();
    const url = "http://127.0.0.1/secret?" + data;
    console.log(url);
    http.open(method, url);
    http.send();
    http.onreadystatechange = function () {
        if (this.status === 200) {
            res.className = "success"
            res.innerHTML = http.responseText
            diff_form.reset()
        } else {
            alert("something went wrong, dude");
        }
    }
}

solveBtn.addEventListener('click', (e) => {
    handle_arithmetic()
});
solveDotProductBtn.addEventListener('click', (e) => {
    handle_vector_dot_product()
});

solveCrossProductBtn.addEventListener('click', (e) => {
    handle_vector_cross_product()
});

