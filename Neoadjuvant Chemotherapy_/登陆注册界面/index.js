const container = document.querySelector('#container');
const signInButton = document.querySelector('#signIn');
const signUpButton = document.querySelector('#signUp');
const registrationForm = document.querySelector('#registrationForm');
const wodeyishengButton = document.querySelector('#wodeyishengButton');
const huanzheguanliButton = document.querySelector('#huanzheguanliButton');
const guanliyuanButton = document.querySelector('#guanliyuanButton');
const uniqueCodeField = document.querySelector('#uniqueCodeField');

// 点击"用户登录"按钮
signInButton.addEventListener('click', () => {
    container.classList.remove('right-panel-active');
});

// 点击"用户注册"按钮
signUpButton.addEventListener('click', () => {
    container.classList.add('right-panel-active');
});

// 点击"wodeyisheng"按钮
wodeyishengButton.addEventListener('click', () => {
    // 隐藏唯一标识码输入字段
    uniqueCodeField.style.display = 'block';
});

// 点击"huanzheguanli"按钮
huanzheguanliButton.addEventListener('click', () => {
    // 显示唯一标识码输入字段
    uniqueCodeField.style.display = 'none';
});

// 点击"guanliyuan"按钮
guanliyuanButton.addEventListener('click', () => {
    // 显示唯一标识码输入字段
    uniqueCodeField.style.display = 'block';
});

// 处理注册按钮的点击事件
const registerButton = document.querySelector('#registerButton');
registerButton.addEventListener('click', (e) => {
    e.preventDefault(); // 防止表单提交
    // 在这里执行注册逻辑，根据用户选择的身份和输入字段的值
    const username = registrationForm.querySelector('input[type="text"]').value;
    const password = registrationForm.querySelector('input[type="password"]').value;
    const email = registrationForm.querySelector('input[type="email"]').value;
    const uniqueCode = registrationForm.querySelector('input[type="text"]').value;
    // 在这里进行注册逻辑，根据上述变量的值进行不同的处理
});
