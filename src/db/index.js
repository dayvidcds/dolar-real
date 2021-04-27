require('dotenv').config();

const mongoose = require('mongoose');

mongoose.Promise = global.Promise;

const connection = process.env.DB_HOST;

/* console.log(process.env.DB_HOST) */

mongoose
    .connect(connection, {
        useNewUrlParser: true,
        useCreateIndex: true,
        useFindAndModify: true,
        useUnifiedTopology: true
        /* 
            user: process.env.DB_USER,
            pass: process.env.DB_PASS */
    })
    .then(res => {
        console.log('MongoDB connection successfull!');
    })
    .catch(reason => {
        console.error(reason);
    });