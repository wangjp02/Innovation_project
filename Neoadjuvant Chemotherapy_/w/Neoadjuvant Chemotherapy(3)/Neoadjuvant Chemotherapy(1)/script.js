const patientsList = document.querySelector('.patient-list');
const addPatientButton = document.querySelector('.add-patient');
const deletePatientButton = document.querySelector('.delete-patient');

// 点击增加按钮时，添加新患者
addPatientButton.addEventListener('click', () => {
    const newPatient = document.createElement('li');
    newPatient.textContent = '新患者'; // 新患者的默认信息
    newPatient.classList.add('patient');
    patientsList.appendChild(newPatient);
});

// 点击删除按钮时，删除最后一个患者
deletePatientButton.addEventListener('click', () => {
    const patients = document.querySelectorAll('.patient');
    if (patients.length > 0) {
        const lastPatient = patients[patients.length - 1];
        patientsList.removeChild(lastPatient);
    }
});

function goBack() {
    window.history.back(); // 使用浏览器的历史记录返回上一页
}