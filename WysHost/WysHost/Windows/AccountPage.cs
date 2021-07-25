using Client;
using Newtonsoft.Json;
using System;
using System.Text;
using System.Windows.Forms;
using WysHost.Windows;

namespace WysHost
{
    public partial class AccountPage : Form
    {
        public static Account _currAccount;
        public AccountPage(Account acc)
        {
            InitializeComponent();
            _currAccount = acc;
            this.Text = this.Text.Replace("NAME", acc.FullName);
            balanceLbl.Text = "Balance: " + _currAccount.Balance.ToString();
            accNumLbl.Text = "Account Number: " + _currAccount.accNum.ToString();
        }

        private void updateBtn_Click(object sender, EventArgs e) => new UpdateAccount(_currAccount).ShowDialog();

        private void logoutBtn_Click(object sender, EventArgs e) => Close();

        private void otpCheck_Click(object sender, EventArgs e)
        {
            int unix = (int)(DateTime.UtcNow.Subtract(new DateTime(1970, 1, 1))).TotalSeconds / Utils.INTERVAL;
            string otp = Encoding.UTF8.GetString(MainWin.connector.SendAndRecvDAL(Encoding.ASCII.GetBytes(unix.ToString()), cmdID.getBase32OTP));
            var OTPData = new { otp = otp };
            string jsonOTP = JsonConvert.SerializeObject(OTPData);
            string ans = MainWin.connector.SendAndRecvServer(serverOpcode.otp, jsonOTP);
            MessageBox.Show(ans);
        }

        private void withdrawBtn_Click(object sender, EventArgs e)
        {
            new Form1().ShowDialog();
            //new withdrawDepositWin(true).ShowDialog();
            balanceLbl.Text = "Balance: " + _currAccount.Balance.ToString();
        }

        private void depositBtn_Click(object sender, EventArgs e)
        {
            new withdrawDepositWin(false).ShowDialog();
            balanceLbl.Text = "Balance: " + _currAccount.Balance.ToString();
        }

        private void transferBtn_Click(object sender, EventArgs e)
        {
            new TransferWin().ShowDialog();
        }
    }
}
