const express = require('express');
const router = express.Router();
const connection = require('./database/db');

// Ruta para la página principal
router.get('/', (req, res) => {
    res.render('welcome.ejs');
});

// Ruta para manejar el post del ID
router.post('/', (req, res) => {  
    const id = req.body.id;
    if (!id) { // Si no se proporciona ningún ID
        res.render('error.ejs', { message: 'Por favor, proporciona un ID válido.' });
    } else {
        connection.query('SELECT name, entry_time FROM employee WHERE id = ?', [id], (error, results) => {
            if (error) {
                console.error('Error al ejecutar la consulta:', error);
                res.status(500).json({ error: 'Error interno del servidor' });
            } else {
                if (results.length > 0) { // Si se encuentra el empleado con el ID proporcionado
                    res.render('attendance.ejs', { results: results });
                } else { // Si no se encuentra ningún empleado con el ID proporcionado
                    res.render('error.ejs', { message: 'No se encontró ningún empleado con el ID proporcionado.' });
                }
            }
        });
    }
});

module.exports = router;
