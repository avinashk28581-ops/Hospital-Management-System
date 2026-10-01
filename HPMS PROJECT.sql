CREATE DATABASE HospitalDB;
USE HospitalDB;
SELECT DATABASE();

CREATE TABLE Departments (
    department_id INT PRIMARY KEY AUTO_INCREMENT,
    department_name VARCHAR(100) NOT NULL,
    description VARCHAR(255)
);

CREATE TABLE Doctors (
    doctor_id INT PRIMARY KEY AUTO_INCREMENT,
    doctor_name VARCHAR(100) NOT NULL,
    specialization VARCHAR(100),
    phone VARCHAR(15),
    department_id INT,
    FOREIGN KEY (department_id)
        REFERENCES Departments(department_id)
);

CREATE TABLE Patients (
    patient_id INT PRIMARY KEY AUTO_INCREMENT,
    patient_name VARCHAR(100) NOT NULL,
    age INT,
    gender VARCHAR(10),
    phone VARCHAR(15),
    address VARCHAR(255),
    blood_group VARCHAR(5)
);

CREATE TABLE Appointments (
    appointment_id INT PRIMARY KEY AUTO_INCREMENT,
    patient_id INT NOT NULL,
    doctor_id INT NOT NULL,
    appointment_date DATE NOT NULL,
    appointment_time TIME NOT NULL,
    status VARCHAR(20) DEFAULT 'Scheduled',

    FOREIGN KEY (patient_id)
        REFERENCES Patients(patient_id),

    FOREIGN KEY (doctor_id)
        REFERENCES Doctors(doctor_id)
);

CREATE TABLE Treatments (
    treatment_id INT PRIMARY KEY AUTO_INCREMENT,
    appointment_id INT NOT NULL,
    diagnosis VARCHAR(255),
    treatment_details VARCHAR(500),
    treatment_date DATE NOT NULL,

    FOREIGN KEY (appointment_id)
        REFERENCES Appointments(appointment_id)
);

CREATE TABLE Medicines (
    medicine_id INT PRIMARY KEY AUTO_INCREMENT,
    medicine_name VARCHAR(100) NOT NULL,
    dosage VARCHAR(50),
    stock_quantity INT DEFAULT 0,
    price DECIMAL(10,2) NOT NULL
);

CREATE TABLE Prescriptions (
    prescription_id INT PRIMARY KEY AUTO_INCREMENT,
    patient_id INT NOT NULL,
    doctor_id INT NOT NULL,
    prescription_date DATE NOT NULL,
    notes VARCHAR(500),

    FOREIGN KEY (patient_id)
        REFERENCES Patients(patient_id),

    FOREIGN KEY (doctor_id)
        REFERENCES Doctors(doctor_id)
);

CREATE TABLE Prescription_Items (
    prescription_id INT NOT NULL,
    medicine_id INT NOT NULL,
    dosage VARCHAR(50),
    quantity INT NOT NULL,

    PRIMARY KEY (prescription_id, medicine_id),

    FOREIGN KEY (prescription_id)
        REFERENCES Prescriptions(prescription_id),

    FOREIGN KEY (medicine_id)
        REFERENCES Medicines(medicine_id)
);

CREATE TABLE Bills (
    bill_id INT PRIMARY KEY AUTO_INCREMENT,
    patient_id INT NOT NULL,
    appointment_id INT,
    amount DECIMAL(10,2) NOT NULL,
    bill_date DATE NOT NULL,
    payment_status VARCHAR(20) DEFAULT 'Pending',

    FOREIGN KEY (patient_id)
        REFERENCES Patients(patient_id),

    FOREIGN KEY (appointment_id)
        REFERENCES Appointments(appointment_id)
);

INSERT INTO Departments (department_name, description)
VALUES
('Cardiology', 'Heart related treatments'),
('Neurology', 'Brain and nervous system treatments'),
('Orthopedics', 'Bone and joint treatments'),
('Pediatrics', 'Child healthcare'),
('General Medicine', 'General medical care');

SELECT * FROM Departments;

INSERT INTO Doctors
(doctor_name, specialization, phone, department_id)
VALUES
('Dr. Ravi Kumar', 'Cardiologist', '9876543210', 1),
('Dr. Priya Sharma', 'Neurologist', '9876543211', 2),
('Dr. Arun Kumar', 'Orthopedic', '9876543212', 3),
('Dr. Sneha Reddy', 'Pediatrician', '9876543213', 4),
('Dr. Kiran Rao', 'General Physician', '9876543214', 5);

INSERT INTO Departments (department_name, description)
VALUES
('Cardiology', 'Heart related treatments'),
('Neurology', 'Brain and nervous system treatments'),
('Orthopedics', 'Bone and joint treatments'),
('Pediatrics', 'Child healthcare'),
('General Medicine', 'General medical care');

SELECT * FROM Departments;

INSERT INTO Doctors
(doctor_name, specialization, phone, department_id)
VALUES
('Dr. Ravi Kumar', 'Cardiologist', '9876543210', 1),
('Dr. Priya Sharma', 'Neurologist', '9876543211', 2),
('Dr. Arun Kumar', 'Orthopedic', '9876543212', 3),
('Dr. Sneha Reddy', 'Pediatrician', '9876543213', 4),
('Dr. Kiran Rao', 'General Physician', '9876543214', 5);

SELECT * FROM Doctors;

INSERT INTO Patients
(patient_name, age, gender, phone, address, blood_group)
VALUES
('Ramesh Kumar', 45, 'Male', '9876500001', 'Hyderabad', 'O+'),
('Sita Devi', 32, 'Female', '9876500002', 'Vijayawada', 'A+'),
('Arjun Reddy', 28, 'Male', '9876500003', 'Guntur', 'B+'),
('Lakshmi Rao', 60, 'Female', '9876500004', 'Warangal', 'AB+'),
('Kiran Kumar', 19, 'Male', '9876500005', 'Nellore', 'O-');

SELECT * FROM Patients;

SELECT patient_id, patient_name
FROM Patients;

SELECT doctor_id, doctor_name
FROM Doctors;

INSERT INTO Appointments
(patient_id, doctor_id, appointment_date, appointment_time, status)
VALUES
(1, 11, '2026-09-03', '10:00:00', 'Scheduled'),
(2, 12, '2026-09-03', '11:00:00', 'Scheduled'),
(3, 13, '2026-09-04', '10:30:00', 'Scheduled'),
(4, 14, '2026-09-04', '12:00:00', 'Scheduled'),
(5, 15, '2026-09-05', '09:30:00', 'Scheduled');

SELECT * FROM Appointments;

INSERT INTO Treatments
(appointment_id, diagnosis, treatment_details, treatment_date)
VALUES
(1, 'Chest Pain', 'ECG and medication advised', '2026-09-03'),
(2, 'Headache', 'Neurological examination and medication', '2026-09-03'),
(3, 'Knee Pain', 'X-Ray and physiotherapy advised', '2026-09-04'),
(4, 'Fever', 'Medication and rest advised', '2026-09-04'),
(5, 'General Weakness', 'Blood test and supplements advised', '2026-09-05');



INSERT INTO Treatments
(appointment_id, diagnosis, treatment_details, treatment_date)
VALUES
(1, 'Chest Pain', 'ECG and medication advised', '2026-09-03'),
(2, 'Headache', 'Neurological examination and medication', '2026-09-03'),
(3, 'Knee Pain', 'X-Ray and physiotherapy advised', '2026-09-04'),
(4, 'Fever', 'Medication and rest advised', '2026-09-04'),
(5, 'General Weakness', 'Blood test and supplements advised', '2026-09-05');

SELECT appointment_id, patient_id, doctor_id
FROM Appointments;

INSERT INTO Treatments
(appointment_id, diagnosis, treatment_details, treatment_date)
VALUES
(6, 'Chest Pain', 'ECG and medication advised', '2026-09-03'),
(7, 'Headache', 'Neurological examination and medication', '2026-09-03'),
(8, 'Knee Pain', 'X-Ray and physiotherapy advised', '2026-09-04'),
(9, 'Fever', 'Medication and rest advised', '2026-09-04'),
(10, 'General Weakness', 'Blood test and supplements advised', '2026-09-05');

SELECT * FROM Treatments;

INSERT INTO Medicines
(medicine_name, dosage, stock_quantity, price)
VALUES
('Paracetamol', '500mg', 100, 5.00),
('Amoxicillin', '250mg', 50, 12.00),
('Ibuprofen', '400mg', 75, 8.00),
('Azithromycin', '500mg', 40, 15.00),
('Vitamin D3', '1000IU', 60, 10.00);

SELECT * FROM Medicines;

INSERT INTO Prescriptions
(patient_id, doctor_id, prescription_date, notes)
VALUES
(1, 11, '2026-09-03', 'Take medicines after food'),
(2, 12, '2026-09-03', 'Take medicines as prescribed'),
(3, 13, '2026-09-04', 'Rest and follow medication'),
(4, 14, '2026-09-04', 'Complete the prescribed course'),
(5, 15, '2026-09-05', 'Take supplements regularly');

SELECT * FROM Prescriptions;

INSERT INTO Prescription_Items
(prescription_id, medicine_id, dosage, quantity)
VALUES
(1, 1, '500mg - Twice daily', 10),
(1, 3, '400mg - Once daily', 5),
(2, 2, '250mg - Three times daily', 15),
(3, 4, '500mg - Once daily', 5),
(4, 1, '500mg - Twice daily', 10),
(5, 5, '1000IU - Once daily', 10);

SELECT * FROM Prescription_Items;

INSERT INTO Bills
(patient_id, appointment_id, amount, bill_date, payment_status)
VALUES
(1, 6, 1500.00, '2026-09-03', 'Paid'),
(2, 7, 1200.00, '2026-09-03', 'Paid'),
(3, 8, 2000.00, '2026-09-04', 'Pending'),
(4, 9, 1000.00, '2026-09-04', 'Paid'),
(5, 10, 800.00, '2026-09-05', 'Pending');

SELECT * FROM Bills;

UPDATE Patients
SET address = 'Hyderabad'
WHERE patient_id = 1;

SELECT * FROM Patients
WHERE patient_id = 1;

INSERT INTO Patients
(patient_name, age, gender, phone, address, blood_group)
VALUES
('Test Patient', 30, 'Male', '9000000000', 'Test City', 'O+');

SELECT * FROM Patients
WHERE patient_name = 'Test Patient';

DELETE FROM Patients
WHERE patient_name = 'Test Patient';

SELECT patient_id, patient_name
FROM Patients
WHERE patient_name = 'Test Patient';

DELETE FROM Patients
WHERE patient_id = 6;

SELECT * FROM Patients
WHERE patient_id = 6;

SELECT * FROM Patients;

SELECT *
FROM Patients
WHERE blood_group = 'O+';

SELECT patient_id, patient_name, age
FROM Patients
ORDER BY age DESC;

SELECT
    p.patient_id,
    p.patient_name,
    d.doctor_name,
    d.specialization,
    a.appointment_date,
    a.appointment_time,
    a.status
FROM Patients p
JOIN Appointments a
    ON p.patient_id = a.patient_id
JOIN Doctors d
    ON a.doctor_id = d.doctor_id;
    
    CREATE VIEW Patient_Appointment_View AS
SELECT
    p.patient_id,
    p.patient_name,
    d.doctor_name,
    d.specialization,
    a.appointment_date,
    a.appointment_time,
    a.status
FROM Patients p
JOIN Appointments a
    ON p.patient_id = a.patient_id
JOIN Doctors d
    ON a.doctor_id = d.doctor_id;
    
    SELECT * FROM Patient_Appointment_View;
    
    SHOW TABLES;
    
    SHOW FULL TABLES
WHERE TABLE_TYPE = 'VIEW';

SHOW CREATE TABLE Patients;

SHOW CREATE TABLE Doctors;

SELECT
    TABLE_NAME,
    COLUMN_NAME,
    CONSTRAINT_NAME,
    REFERENCED_TABLE_NAME,
    REFERENCED_COLUMN_NAME
FROM INFORMATION_SCHEMA.KEY_COLUMN_USAGE
WHERE TABLE_SCHEMA = 'HospitalDB'
  AND REFERENCED_TABLE_NAME IS NOT NULL;