-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Feb 04, 2024 at 09:42 PM
-- Server version: 10.4.28-MariaDB
-- PHP Version: 8.2.4

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `trolly`
--

-- --------------------------------------------------------

--
-- Table structure for table `trollymanager`
--

CREATE TABLE `trollymanager` (
  `id` int(11) NOT NULL,
  `scanning_status` int(11) NOT NULL,
  `selected_station` int(11) NOT NULL,
  `moving_status` int(11) NOT NULL,
  `trolly_status` int(11) NOT NULL,
  `obstacle_status` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `trollymanager`
--

INSERT INTO `trollymanager` (`id`, `scanning_status`, `selected_station`, `moving_status`, `trolly_status`, `obstacle_status`) VALUES
(1, 1, 2, 1, 1, 0),
(2, 1, 3, 1, 1, 0),
(3, 1, 4, 1, 1, 0),
(4, 1, 4, 1, 1, 1),
(5, 1, 4, 1, 1, 1),
(6, 1, 4, 1, 0, 1),
(7, 1, 2, 1, 0, 1),
(8, 1, 2, 1, 1, 1),
(9, 1, 2, 1, 1, 1),
(10, 0, 2, 1, 0, 1),
(11, 0, 2, 1, 1, 1),
(12, 0, 2, 1, 0, 1),
(13, 0, 2, 1, 1, 1);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `trollymanager`
--
ALTER TABLE `trollymanager`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `trollymanager`
--
ALTER TABLE `trollymanager`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=14;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
