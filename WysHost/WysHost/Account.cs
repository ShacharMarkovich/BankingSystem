using System;

namespace WysHost
{
    public class Account
    {
        public string Username { get; set; }
        public string Password { get; set; }
        public string Email { get; set; }
        public string FullName { get; set; }
        public DateTime BirthDay { get; set; }
        public string Country { get; set; }
        public string City { get; set; }
        public string Street { get; set; }
        public int HouseNum { get; set; }
        public string Gender { get; set; }
        public bool IsMarry { get; set; }

        public override string ToString() => "Account: " + Username + " " + Password;
    }
}
