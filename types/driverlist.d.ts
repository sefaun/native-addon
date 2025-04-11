declare module "driverlist" {
  type Drive = { letter: string; type: string }

  const DriverList: {
    getDrives(): Drive[]
  }

  export default DriverList
}
