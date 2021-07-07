using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WysHost
{
    public partial class RegisterWin : Form
    {
        public RegisterWin()
        {
            InitializeComponent();
            showPassBtn.BackgroundImage = Image.FromFile("../../Images/show.jpg");
            showPassBtn.BackgroundImageLayout = ImageLayout.Stretch;
            passwordTextBox.UseSystemPasswordChar = true;
        }

        private void retBtn_Click(object sender, EventArgs e) => Close();

        private void clearBtn_Click(object sender, EventArgs e) => accountBindingSource.Clear();

        bool IsValidEmail(string email)
        {
            try
            {
                var addr = new System.Net.Mail.MailAddress(email);
                return addr.Address == email;
            }
            catch
            {
                return false;
            }
        }

        private Account GetNewAccountData()
        {
            Account newAcc = new Account();
            newAcc.Username = usernameTextBox.Text;
            newAcc.Password = passwordTextBox.Text;
            newAcc.Email = emailTextBox.Text;
            newAcc.FullName = fullNameTextBox.Text;
            newAcc.BirthDay = birthDayDateTimePicker.Value.Date;
            newAcc.Country = countryTextBox.Text;
            newAcc.City = cityTextBox.Text;
            newAcc.Street = streetTextBox.Text;
            newAcc.Gender = genderComboBox.SelectedItem as string;
            newAcc.IsMarry = isMarryCheckBox.Checked;
            int num = -1;
            int.TryParse(houseNumTextBox.Text, out num);
            newAcc.HouseNum = num;

            return newAcc;
        }

        private bool CheckNewAccountData(Account newAcc)
        {
            return newAcc.Username != "" && newAcc.Password != "" && IsValidEmail(newAcc.Email) && newAcc.FullName != "" &&
                newAcc.Country != "" && newAcc.City != "" && newAcc.Street != "" && newAcc.HouseNum > 0 &&
                (newAcc.Gender == "Male" || newAcc.Gender == "Female");
        }

        private void submitBtn_Click(object sender, EventArgs e)
        {
            Account newAcc = GetNewAccountData();
            if (CheckNewAccountData(newAcc))
                MessageBox.Show("TODO: Handler ledal data!");
            else
                MessageBox.Show("not all data had been writen legally!\nFix it!");
        }

        private void showPassBtn_Click(object sender, EventArgs e)
        {
            if (passwordTextBox.UseSystemPasswordChar)
                showPassBtn.BackgroundImage = Image.FromFile("../../Images/hide.jpg");
            else
                showPassBtn.BackgroundImage = Image.FromFile("../../Images/show.jpg");

            passwordTextBox.UseSystemPasswordChar = !passwordTextBox.UseSystemPasswordChar;
        }
    }
}
