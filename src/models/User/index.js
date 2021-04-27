const mongoose = require('mongoose');

const {
  ObjectId
} = mongoose.Types;

const schema = mongoose.Schema({
  email: String,
  senha: String,
  nome: String,
  telefone: String,
  deviceToken: String,
  deviceOptions: {
    ledColor: {
      r: Number,
      g: Number,
      b: Number
    },
    moeda: String,
    delayDisplay: Number,
  }
}, {
  timestamps: true,
  autoCreate: true
});

const model = mongoose.model('User', schema);

module.exports = model;