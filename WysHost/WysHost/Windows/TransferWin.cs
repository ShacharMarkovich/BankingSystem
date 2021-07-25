using System.Windows.Forms;

namespace WysHost.Windows
{
    public partial class TransferWin : Form
    {
        Account _currAccount;
        public TransferWin(Account acc)
        {
            InitializeComponent();
            _currAccount = acc;
        }
    }
}
