
import Axios from 'axios';

const pressLeft = async () => Axios.post('http://127.0.0.1:5000/button/left', { "action": "press-and-release" });
const pressRight = async () => Axios.post('http://127.0.0.1:5000/button/right', { "action": "press-and-release" });
const pressBoth = async () => Axios.post('http://127.0.0.1:5000/button/both', { "action": "press-and-release" });

export {
    pressLeft, pressRight, pressBoth
};