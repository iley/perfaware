package main

import (
	"fmt"
	"io"
	"log"
	"os"
	"strings"
)

func main() {
	if len(os.Args) < 2 {
		log.Fatalf("usage: %s file", os.Args[0])
	}

	inputPath := os.Args[1]

	bytes, err := ReadFile(inputPath)
	if err != nil {
		log.Fatalf("could not read file %s: %v", inputPath, err)
	}

	output, err := Disassemble(bytes)
	if err != nil {
		log.Fatalf("disassembly error: %v", err)
	}

	fmt.Printf("; disassembly for file %s\n", inputPath)
	fmt.Print(output)
}

func ReadFile(path string) ([]byte, error) {
	fd, err := os.Open(path)
	if err != nil {
		return nil, err
	}

	return io.ReadAll(fd)
}

func Disassemble(binary []byte) (string, error) {
	var sb strings.Builder

	sb.WriteString("bits 16\n")

	offset := 0
	for offset < len(binary) {
		bytesRead, err := DisassembleInstruction(binary[offset:], &sb)
		if err != nil {
			return "", err
		}

		offset += bytesRead
	}

	return sb.String(), nil
}

func DisassembleInstruction(binary []byte, sb *strings.Builder) (int, error) {
	firstByte := binary[0]

	top6bits := firstByte >> 2

	if top6bits != 0b100010 {
		return 0, fmt.Errorf("unsupported instruction")
	}

	flagD := (firstByte >> 1) & 1
	flagW := firstByte & 1

	secondByte := binary[1]
	mod := secondByte >> 6
	reg := (secondByte >> 3) & 0b111
	rm := secondByte & 0b111

	if mod != 0b11 {
		return 0, fmt.Errorf("only register-register mode is supported")
	}

	firstRegister, err := decodeRegister(reg, flagW == 1)
	if err != nil {
		return 0, err
	}

	secondRegister, err := decodeRegister(rm, flagW == 1)
	if err != nil {
		return 0, err
	}

	var srcRegister, dstRegister string
	if flagD == 1 {
		dstRegister, srcRegister = firstRegister, secondRegister
	} else {
		dstRegister, srcRegister = secondRegister, firstRegister
	}

	sb.WriteString(fmt.Sprintf("mov %s, %s\n", dstRegister, srcRegister))

	return 2, nil
}

type regKey struct {
	reg byte
	wide bool
}

var registerDecodeTable = map[regKey]string{
	{0b000, false}: "al",
	{0b000, true}: "ax",
	{0b001, false}: "cl",
	{0b001, true}: "cx",
	{0b010, false}: "dl",
	{0b010, true}: "dx",
	{0b011, false}: "bl",
	{0b011, true}: "bx",
	{0b100, false}: "ah",
	{0b100, true}: "sp",
	{0b101, false}: "ch",
	{0b101, true}: "bp",
	{0b110, false}: "dh",
	{0b110, true}: "si",
	{0b111, false}: "bh",
	{0b111, true}: "di",
}

func decodeRegister(reg byte, wide bool) (string, error) {
	register, ok := registerDecodeTable[regKey{reg, wide}]

	if !ok {
		return "", fmt.Errorf("register not found for bit pattern %d wide=%v", reg, wide)
	}

	return register, nil
}
