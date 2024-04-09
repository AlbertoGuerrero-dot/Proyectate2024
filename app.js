const express = require('express');
const app = express();

// Middleware para analizar el cuerpo de la solicitud en formato JSON
app.use(express.json());

// Ruta para manejar las solicitudes
const router = require('./routers');
app.use('/', router);

// Configuración de servidor
const PORT = 3000;
app.listen(PORT, () => {
    console.log(`Servidor en ejecución en el puerto ${PORT}`);
});
