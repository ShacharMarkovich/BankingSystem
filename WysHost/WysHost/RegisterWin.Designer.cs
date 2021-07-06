namespace WysHost
{
    partial class RegisterWin
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.Label birthDayLabel;
            System.Windows.Forms.Label cityLabel;
            System.Windows.Forms.Label countryLabel;
            System.Windows.Forms.Label emailLabel;
            System.Windows.Forms.Label fullNameLabel;
            System.Windows.Forms.Label genderLabel;
            System.Windows.Forms.Label houseNumLabel;
            System.Windows.Forms.Label passwordLabel;
            System.Windows.Forms.Label streetLabel;
            System.Windows.Forms.Label usernameLabel;
            this.birthDayDateTimePicker = new System.Windows.Forms.DateTimePicker();
            this.cityTextBox = new System.Windows.Forms.TextBox();
            this.countryTextBox = new System.Windows.Forms.TextBox();
            this.emailTextBox = new System.Windows.Forms.TextBox();
            this.fullNameTextBox = new System.Windows.Forms.TextBox();
            this.genderComboBox = new System.Windows.Forms.ComboBox();
            this.houseNumTextBox = new System.Windows.Forms.TextBox();
            this.isMarryCheckBox = new System.Windows.Forms.CheckBox();
            this.passwordTextBox = new System.Windows.Forms.TextBox();
            this.streetTextBox = new System.Windows.Forms.TextBox();
            this.usernameTextBox = new System.Windows.Forms.TextBox();
            this.submitBtn = new System.Windows.Forms.Button();
            this.clearBtn = new System.Windows.Forms.Button();
            this.retBtn = new System.Windows.Forms.Button();
            this.instLbl = new System.Windows.Forms.Label();
            this.accountBindingSource = new System.Windows.Forms.BindingSource(this.components);
            birthDayLabel = new System.Windows.Forms.Label();
            cityLabel = new System.Windows.Forms.Label();
            countryLabel = new System.Windows.Forms.Label();
            emailLabel = new System.Windows.Forms.Label();
            fullNameLabel = new System.Windows.Forms.Label();
            genderLabel = new System.Windows.Forms.Label();
            houseNumLabel = new System.Windows.Forms.Label();
            passwordLabel = new System.Windows.Forms.Label();
            streetLabel = new System.Windows.Forms.Label();
            usernameLabel = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.accountBindingSource)).BeginInit();
            this.SuspendLayout();
            // 
            // birthDayLabel
            // 
            birthDayLabel.AutoSize = true;
            birthDayLabel.Location = new System.Drawing.Point(85, 175);
            birthDayLabel.Name = "birthDayLabel";
            birthDayLabel.Size = new System.Drawing.Size(53, 13);
            birthDayLabel.TabIndex = 1;
            birthDayLabel.Text = "Birth Day:";
            // 
            // cityLabel
            // 
            cityLabel.AutoSize = true;
            cityLabel.Location = new System.Drawing.Point(85, 253);
            cityLabel.Name = "cityLabel";
            cityLabel.Size = new System.Drawing.Size(27, 13);
            cityLabel.TabIndex = 3;
            cityLabel.Text = "City:";
            // 
            // countryLabel
            // 
            countryLabel.AutoSize = true;
            countryLabel.Location = new System.Drawing.Point(85, 227);
            countryLabel.Name = "countryLabel";
            countryLabel.Size = new System.Drawing.Size(46, 13);
            countryLabel.TabIndex = 5;
            countryLabel.Text = "Country:";
            // 
            // emailLabel
            // 
            emailLabel.AutoSize = true;
            emailLabel.Location = new System.Drawing.Point(85, 148);
            emailLabel.Name = "emailLabel";
            emailLabel.Size = new System.Drawing.Size(35, 13);
            emailLabel.TabIndex = 7;
            emailLabel.Text = "Email:";
            // 
            // fullNameLabel
            // 
            fullNameLabel.AutoSize = true;
            fullNameLabel.Location = new System.Drawing.Point(85, 70);
            fullNameLabel.Name = "fullNameLabel";
            fullNameLabel.Size = new System.Drawing.Size(57, 13);
            fullNameLabel.TabIndex = 9;
            fullNameLabel.Text = "Full Name:";
            // 
            // genderLabel
            // 
            genderLabel.AutoSize = true;
            genderLabel.Location = new System.Drawing.Point(85, 200);
            genderLabel.Name = "genderLabel";
            genderLabel.Size = new System.Drawing.Size(45, 13);
            genderLabel.TabIndex = 11;
            genderLabel.Text = "Gender:";
            // 
            // houseNumLabel
            // 
            houseNumLabel.AutoSize = true;
            houseNumLabel.Location = new System.Drawing.Point(85, 305);
            houseNumLabel.Name = "houseNumLabel";
            houseNumLabel.Size = new System.Drawing.Size(48, 13);
            houseNumLabel.TabIndex = 13;
            houseNumLabel.Text = "House#:";
            // 
            // passwordLabel
            // 
            passwordLabel.AutoSize = true;
            passwordLabel.Location = new System.Drawing.Point(85, 122);
            passwordLabel.Name = "passwordLabel";
            passwordLabel.Size = new System.Drawing.Size(56, 13);
            passwordLabel.TabIndex = 17;
            passwordLabel.Text = "Password:";
            // 
            // streetLabel
            // 
            streetLabel.AutoSize = true;
            streetLabel.Location = new System.Drawing.Point(85, 279);
            streetLabel.Name = "streetLabel";
            streetLabel.Size = new System.Drawing.Size(38, 13);
            streetLabel.TabIndex = 19;
            streetLabel.Text = "Street:";
            // 
            // usernameLabel
            // 
            usernameLabel.AutoSize = true;
            usernameLabel.Location = new System.Drawing.Point(85, 96);
            usernameLabel.Name = "usernameLabel";
            usernameLabel.Size = new System.Drawing.Size(58, 13);
            usernameLabel.TabIndex = 21;
            usernameLabel.Text = "Username:";
            // 
            // birthDayDateTimePicker
            // 
            this.birthDayDateTimePicker.DataBindings.Add(new System.Windows.Forms.Binding("Value", this.accountBindingSource, "BirthDay", true));
            this.birthDayDateTimePicker.Location = new System.Drawing.Point(157, 171);
            this.birthDayDateTimePicker.Name = "birthDayDateTimePicker";
            this.birthDayDateTimePicker.Size = new System.Drawing.Size(200, 20);
            this.birthDayDateTimePicker.TabIndex = 2;
            // 
            // cityTextBox
            // 
            this.cityTextBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.accountBindingSource, "City", true));
            this.cityTextBox.Location = new System.Drawing.Point(157, 250);
            this.cityTextBox.Name = "cityTextBox";
            this.cityTextBox.Size = new System.Drawing.Size(200, 20);
            this.cityTextBox.TabIndex = 4;
            // 
            // countryTextBox
            // 
            this.countryTextBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.accountBindingSource, "Country", true));
            this.countryTextBox.Location = new System.Drawing.Point(157, 224);
            this.countryTextBox.Name = "countryTextBox";
            this.countryTextBox.Size = new System.Drawing.Size(200, 20);
            this.countryTextBox.TabIndex = 6;
            // 
            // emailTextBox
            // 
            this.emailTextBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.accountBindingSource, "Email", true));
            this.emailTextBox.Location = new System.Drawing.Point(157, 145);
            this.emailTextBox.Name = "emailTextBox";
            this.emailTextBox.Size = new System.Drawing.Size(200, 20);
            this.emailTextBox.TabIndex = 8;
            // 
            // fullNameTextBox
            // 
            this.fullNameTextBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.accountBindingSource, "FullName", true));
            this.fullNameTextBox.Location = new System.Drawing.Point(157, 67);
            this.fullNameTextBox.Name = "fullNameTextBox";
            this.fullNameTextBox.Size = new System.Drawing.Size(200, 20);
            this.fullNameTextBox.TabIndex = 10;
            // 
            // genderComboBox
            // 
            this.genderComboBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.accountBindingSource, "Gender", true));
            this.genderComboBox.FormattingEnabled = true;
            this.genderComboBox.Items.AddRange(new object[] {
            "Male",
            "Female"});
            this.genderComboBox.Location = new System.Drawing.Point(157, 197);
            this.genderComboBox.Name = "genderComboBox";
            this.genderComboBox.Size = new System.Drawing.Size(200, 21);
            this.genderComboBox.TabIndex = 12;
            // 
            // houseNumTextBox
            // 
            this.houseNumTextBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.accountBindingSource, "HouseNum", true));
            this.houseNumTextBox.Location = new System.Drawing.Point(157, 302);
            this.houseNumTextBox.Name = "houseNumTextBox";
            this.houseNumTextBox.Size = new System.Drawing.Size(200, 20);
            this.houseNumTextBox.TabIndex = 14;
            // 
            // isMarryCheckBox
            // 
            this.isMarryCheckBox.DataBindings.Add(new System.Windows.Forms.Binding("CheckState", this.accountBindingSource, "IsMarry", true));
            this.isMarryCheckBox.Location = new System.Drawing.Point(157, 328);
            this.isMarryCheckBox.Name = "isMarryCheckBox";
            this.isMarryCheckBox.Size = new System.Drawing.Size(200, 24);
            this.isMarryCheckBox.TabIndex = 16;
            this.isMarryCheckBox.Text = "Is Marry?";
            this.isMarryCheckBox.UseVisualStyleBackColor = true;
            // 
            // passwordTextBox
            // 
            this.passwordTextBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.accountBindingSource, "Password", true));
            this.passwordTextBox.Location = new System.Drawing.Point(157, 119);
            this.passwordTextBox.Name = "passwordTextBox";
            this.passwordTextBox.Size = new System.Drawing.Size(200, 20);
            this.passwordTextBox.TabIndex = 18;
            // 
            // streetTextBox
            // 
            this.streetTextBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.accountBindingSource, "Street", true));
            this.streetTextBox.Location = new System.Drawing.Point(157, 276);
            this.streetTextBox.Name = "streetTextBox";
            this.streetTextBox.Size = new System.Drawing.Size(200, 20);
            this.streetTextBox.TabIndex = 20;
            // 
            // usernameTextBox
            // 
            this.usernameTextBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.accountBindingSource, "Username", true));
            this.usernameTextBox.Location = new System.Drawing.Point(157, 93);
            this.usernameTextBox.Name = "usernameTextBox";
            this.usernameTextBox.Size = new System.Drawing.Size(200, 20);
            this.usernameTextBox.TabIndex = 22;
            // 
            // submitBtn
            // 
            this.submitBtn.Location = new System.Drawing.Point(88, 400);
            this.submitBtn.Name = "submitBtn";
            this.submitBtn.Size = new System.Drawing.Size(75, 23);
            this.submitBtn.TabIndex = 23;
            this.submitBtn.Text = "Register";
            this.submitBtn.UseVisualStyleBackColor = true;
            this.submitBtn.Click += new System.EventHandler(this.submitBtn_Click);
            // 
            // clearBtn
            // 
            this.clearBtn.Location = new System.Drawing.Point(180, 400);
            this.clearBtn.Name = "clearBtn";
            this.clearBtn.Size = new System.Drawing.Size(75, 23);
            this.clearBtn.TabIndex = 24;
            this.clearBtn.Text = "Clear";
            this.clearBtn.UseVisualStyleBackColor = true;
            this.clearBtn.Click += new System.EventHandler(this.clearBtn_Click);
            // 
            // retBtn
            // 
            this.retBtn.Location = new System.Drawing.Point(282, 400);
            this.retBtn.Name = "retBtn";
            this.retBtn.Size = new System.Drawing.Size(75, 23);
            this.retBtn.TabIndex = 25;
            this.retBtn.Text = "Return";
            this.retBtn.UseVisualStyleBackColor = true;
            this.retBtn.Click += new System.EventHandler(this.retBtn_Click);
            // 
            // instLbl
            // 
            this.instLbl.AutoSize = true;
            this.instLbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(177)));
            this.instLbl.Location = new System.Drawing.Point(12, 9);
            this.instLbl.Name = "instLbl";
            this.instLbl.Size = new System.Drawing.Size(438, 20);
            this.instLbl.TabIndex = 26;
            this.instLbl.Text = "Please fill the data below in order to compete the registration:";
            // 
            // accountBindingSource
            // 
            this.accountBindingSource.DataSource = typeof(WysHost.Account);
            // 
            // RegisterWin
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(457, 448);
            this.Controls.Add(this.instLbl);
            this.Controls.Add(this.retBtn);
            this.Controls.Add(this.clearBtn);
            this.Controls.Add(this.submitBtn);
            this.Controls.Add(birthDayLabel);
            this.Controls.Add(this.birthDayDateTimePicker);
            this.Controls.Add(cityLabel);
            this.Controls.Add(this.cityTextBox);
            this.Controls.Add(countryLabel);
            this.Controls.Add(this.countryTextBox);
            this.Controls.Add(emailLabel);
            this.Controls.Add(this.emailTextBox);
            this.Controls.Add(fullNameLabel);
            this.Controls.Add(this.fullNameTextBox);
            this.Controls.Add(genderLabel);
            this.Controls.Add(this.genderComboBox);
            this.Controls.Add(houseNumLabel);
            this.Controls.Add(this.houseNumTextBox);
            this.Controls.Add(this.isMarryCheckBox);
            this.Controls.Add(passwordLabel);
            this.Controls.Add(this.passwordTextBox);
            this.Controls.Add(streetLabel);
            this.Controls.Add(this.streetTextBox);
            this.Controls.Add(usernameLabel);
            this.Controls.Add(this.usernameTextBox);
            this.Name = "RegisterWin";
            this.Text = "Register Window";
            ((System.ComponentModel.ISupportInitialize)(this.accountBindingSource)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.BindingSource accountBindingSource;
        private System.Windows.Forms.DateTimePicker birthDayDateTimePicker;
        private System.Windows.Forms.TextBox cityTextBox;
        private System.Windows.Forms.TextBox countryTextBox;
        private System.Windows.Forms.TextBox emailTextBox;
        private System.Windows.Forms.TextBox fullNameTextBox;
        private System.Windows.Forms.ComboBox genderComboBox;
        private System.Windows.Forms.TextBox houseNumTextBox;
        private System.Windows.Forms.CheckBox isMarryCheckBox;
        private System.Windows.Forms.TextBox passwordTextBox;
        private System.Windows.Forms.TextBox streetTextBox;
        private System.Windows.Forms.TextBox usernameTextBox;
        private System.Windows.Forms.Button submitBtn;
        private System.Windows.Forms.Button clearBtn;
        private System.Windows.Forms.Button retBtn;
        private System.Windows.Forms.Label instLbl;
    }
}