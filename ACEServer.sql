-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

-- -----------------------------------------------------
-- Schema ACEDB
-- -----------------------------------------------------
DROP SCHEMA IF EXISTS `ACEDB` ;

-- -----------------------------------------------------
-- Schema ACEDB
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `ACEDB` DEFAULT CHARACTER SET utf8 ;
SHOW WARNINGS;
USE `ACEDB` ;

-- -----------------------------------------------------
-- Table `ACEDB`.`Glasses ID`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `ACEDB`.`Glasses ID` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `ACEDB`.`Glasses ID` (
  `Glass ID` VARCHAR(45) NOT NULL COMMENT '\n',
  `MAC Address` VARCHAR(45) NULL,
  `IP Address` VARCHAR(45) NULL,
  PRIMARY KEY (`Glass ID`))
ENGINE = InnoDB;

SHOW WARNINGS;
CREATE UNIQUE INDEX `Glass ID_UNIQUE` ON `ACEDB`.`Glasses ID` (`Glass ID` ASC) VISIBLE;

SHOW WARNINGS;
CREATE UNIQUE INDEX `MAC Address_UNIQUE` ON `ACEDB`.`Glasses ID` (`MAC Address` ASC) VISIBLE;

SHOW WARNINGS;
CREATE UNIQUE INDEX `IP Address_UNIQUE` ON `ACEDB`.`Glasses ID` (`IP Address` ASC) VISIBLE;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `ACEDB`.`Picture Table`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `ACEDB`.`Picture Table` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `ACEDB`.`Picture Table` (
  `Picture ID` VARCHAR(45) NOT NULL,
  `Date` TIMESTAMP(45) NULL,
  `Picture` BLOB NULL,
  PRIMARY KEY (`Picture ID`),
  CONSTRAINT `Glass ID`
    FOREIGN KEY (`Picture ID`)
    REFERENCES `ACEDB`.`Glasses ID` (`Glass ID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;
CREATE UNIQUE INDEX `Picture ID_UNIQUE` ON `ACEDB`.`Picture Table` (`Picture ID` ASC) VISIBLE;

SHOW WARNINGS;

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
