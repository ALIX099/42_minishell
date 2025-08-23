# 42 MiniShell 💻⚡

<p align="center">
  <img src="https://github.com/abouknan/abouknan/assets/branding/banner.png" alt="MiniShell Banner" width="80%">
</p>

---

### 🔹 Overview
**42 MiniShell** is a Unix-like shell built from scratch in C.  
It supports command parsing, redirections, pipes, and built-in commands while handling signals and memory safely.  
The project demonstrates **low-level system programming**, **process control**, and **inter-process communication (IPC)**.  

---

### 🛠 Features
- ✅ Command execution with arguments  
- ✅ Built-in commands: `cd`, `echo`, `pwd`, `export`, `unset`, `env`, `exit`  
- ✅ Input/output redirections: `>`, `>>`, `<`  
- ✅ Pipes: `|` for chaining commands  
- ✅ Signal handling: `Ctrl+C`, `Ctrl+D`  
- ✅ Memory-safe execution (no leaks with proper cleanup)  

---

### ⚡ Technical Highlights
- Parsing and tokenization of user input  
- Environment variable management  
- Use of **fork()**, **execve()**, **waitpid()**, and **dup2()**  
- Error handling for invalid commands or redirections  
- Modular code for readability and maintainability  

---

### 🧩 Learning Outcomes
- Understanding Unix system calls  
- Implementing pipes and redirections  
- Handling multiple processes safely  
- Writing modular, maintainable C code  

---

### 🔗 Links
- [GitHub Repository](https://github.com/abouknan/42_minishell)  
- ![42 Badge](https://badge.mediaplus.ma/star/abouknan)
