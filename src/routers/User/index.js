const express = require('express');
const router = express.Router();
const {
  User,
} = require('../../models');

const jwt = require('jsonwebtoken');
const cookieParser = require('cookie-parser');

router.use(cookieParser());

let nomeEmpresa = '';

router.post('/cadastrar', (req, res, next) => {
  const data = req.body;

  User.insert(data)
    .then(resp => {
      if (!resp) return Promise.reject('Erro ao criar usuário!');
      res.status(200).json(resp);
    })
    .catch(resp => next(resp));
});

module.exports = router;