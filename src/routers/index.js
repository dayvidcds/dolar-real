const User = require('./User');

const router = require('express').Router();

router.use('/user', User);

module.exports = router;