var mysql = require ('mysql');

var connection = mysql.createConnection({ //Conexion a la base de datos 
    host: 'localhost',
    database: 'employees',
    user: 'root',
    password: ''
});


connection.connect(function(error){
    if(error){
        throw error;
    }
    else{
        console.log("CONEXION EXITOSA A LA BASE DE DATOS");
    }
});

module.exports = connection;