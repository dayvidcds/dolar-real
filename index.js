require('dotenv').config();
require('./src/db');

const mongoose = require('mongoose');

const ObjectId = mongoose.Types.ObjectId;

const port = process.env.PORT || 3000;

const age = 36000065000 * 24;

const path = require('path');
const cookieParser = require('cookie-parser');
const express = require('express');
const bodyParser = require('body-parser');
/* const helmet = require('helmet');
const morgan = require('morgan'); */

const app = express();
const http = require('http');
const cors = require('cors');

const axios = require('axios');

const jwt = require('jsonwebtoken')

app.set('view engine', 'hbs'); //Handlebars

app.set('views', path.join(__dirname, '/views'));

app.disable('view cache');

const server = http.createServer(app);

const fs = require('fs');

const corsOptions = {
  origin: '*', //FIXME: Colocar para a origem ser apenas do gateway
  optionsSuccessStatus: 200 // some legacy browsers (IE11, various SmartTVs) choke on 204
};

const routers = require('./src/routers');

app.use(bodyParser.urlencoded({
  extended: true
}));

app.use(cors(corsOptions));
app.use(cookieParser());

// app.use(helmet());
// app.use(morgan('combined'));

const DIR = path.join(__dirname, './app/public')

app.use(express.static(__dirname));

app.use('/css', express.static(DIR + '/css'))
app.use('/js', express.static(DIR + '/js'))
app.use('/images', express.static(DIR + '/images'))
app.use('/vendor', express.static(DIR + '/vendor'))

app.use(cookieParser())

//app.use('/app', routers);

const {
  User,
} = require('./src/models');

app.get('/', (req, res) => {
  const cookie = req.cookies.userCookie;
  if (cookie == undefined) {
    res.cookie('userCookie', {
      token: null,
      user: {
        nome: null,
        email_usuario: null,
        email_parceiro: null
      }
    })
    res.sendFile(DIR + '/login.html')
  } else if (cookie.token == null) {
    res.sendFile(DIR + '/login.html')
  } else {
    res.redirect('/profile')
  }
});

app.get('/signup', (req, res) => {
  res.sendFile(DIR + '/signup.html')
});

app.get('/informations', (req, res) => {
  const id = ObjectId(req.cookies.userCookie.user._doc._id);
  User.findById(id).then(user => {
    res.json({
      status: true,
      user: user
    });
  }).catch(error => {
    res.json({
      status: false,
      error: error
    });
  });
});

app.get('/informations/:token', (req, res) => {
  const token = req.params.token;
 // console.log('algo aqui');

  axios.get('http://economia.awesomeapi.com.br/json/all/USD-BRL')
    .then(function (response) {
      // handle success
      console.log(response);
      User.findOne({
        deviceToken: token
      }).then(user => {
        res.json({
          status: true,
          user: user,
          ...response.data
        });
      }).catch(error => {
        res.json({
          status: false,
          error: error
        });
      });
    })
    .catch(function (error) {
      // handle error
      console.log(error);
    })
    .then(function () {
      // always executed
    });

});

app.post('/setColor', (req, res) => {
  const {
    color
  } = req.body;

  let colorTemp = color.split(',');

  const r = colorTemp[0];
  const g = colorTemp[1];
  const b = colorTemp[2];

  const id = ObjectId(req.cookies.userCookie.user._doc._id);

  User.findOneAndUpdate({
    _id: id
  }, {
    'deviceOptions.ledColor': {
      r,
      g,
      b
    }
  }, (err, user) => {
    if (err) {
      res.json({
        status: false
      });
    } else {
      res.json({
        color: color,
        user
      });
    }
  });
});

app.post('/setDelay', (req, res) => {
  const {
    delay
  } = req.body;
  const id = ObjectId(req.cookies.userCookie.user._doc._id);

  User.findOneAndUpdate({
    _id: id
  }, {
    'deviceOptions.delayDisplay': delay
  }, (err, user) => {
    if (err) {
      res.json({
        status: false
      });
    } else {
      res.json({
        delay: delay
      });
    }
  });
});

app.get('/login', (req, res) => {
  return res.sendFile(DIR + '/login.html');
  const cookie = req.cookies.userCookie;
  if (cookie == undefined) {
    res.cookie('userCookie', {
      token: null,
      user: {
        nome: null,
        email_usuario: null,
        email_parceiro: null
      }
    })
    res.sendFile(DIR + '/login.html')
  } else if (cookie.token == null) {
    res.sendFile(DIR + '/login.html')
  } else {
    res.redirect('/profile')
  }
});

app.get('/profile', (req, res) => {
  res.sendFile(DIR + '/profile.html')
});

app.get('/signup', (req, res) => {
  res.sendFile(DIR + '/signup.html')
});

app.get('/logout', (req, res) => {
  res.cookie('userCookie', {
    token: null,
    user: null
  })
  res.redirect('/');
});

app.post('/api/login', (req, res) => {
  const {
    email,
    senha
  } = req.body;

  User.findOne({
    email,
    senha
  }, (err, response) => {

    if (err || response == '' || response == undefined) {
      res.json({
        status: false,
        msg: err
      });
    } else {
      const jwtSecret = 'SECRET';

      const token = jwt.sign(response.toJSON(), jwtSecret, {
        expiresIn: 1440
      })
      res.cookie('userCookie', {
        token: token,
        user: {
          ...response
        }
      })
      res.json({
        status: true,
        msg: response
      });
    }
  });
});

app.get('/teste/:a', (req, res) => {
  res.send(req.params);
});

app.use('*', (req, res, next) => {
  //res.status(404).sendFile(`${__dirname}/pages/public/404.html`);
});

app.use((err, req, res, next) => {
  if (typeof err === 'string') {
    return res.status(400).json({
      msg: err
    });
  }

  if (err.name === 'ValidationError') {
    return res.status(404).json({
      msg: err.message
    });
  }

  if (err.response && err.response.status === 404) {
    return res.status(404).json({
      msg: err.response.data.msg
    });
  }

  if (err.statusCode === 404) {
    return res.status(404).json({
      msg: err.message
    });
  }

  return res.status(500).json({
    msg: err.message
  });
});

server.listen(port, (err, res) => {
  if (err) {
    console.error(error);
    return;
  }
  console.log(`Server Connection SUCCESS Started on port ${port}`);
});