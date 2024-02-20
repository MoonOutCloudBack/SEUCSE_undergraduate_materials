
#funções

def pede_nome():
    return(input("Digite seu nome: "))

def pede_cpf():
    return(input("Digite seu cpf: "))

def pede_matricula():
    return(input("Digite a matricula: "))

def add_professor():
    return(input("Adicionar professor? [S/N] ").upper())
    
def add_aluno():
    return(input("Adicionar aluno? [S/N] ").upper())
    
def add_disciplina():
    return(input("Adicionar disciplina? [S/N] ").upper())
    
def nome_disciplina():
    return(input("Nome da disciplina: "))
    
def codigo_disciplina():
    return(input("Digite o código da disciplina:"))


def add_supremo(parametro):
    funcao = paremetro
    if funcao == "professor":
        input("Adicionar professor? [S/N] ").upper()
    elif funcao == "aluno":
        input("Adicionar aluno? [S/N] ").upper()
    elif funcao == "disciplina":
        input("Adicionar aluno? [S/N] ").upper()


def chamada():
    return(int(input("Digite a opção: ")))

def menu_add():
    print("""
    ░░░░░░░░░ DIGITE O NÚMERO PARA ADICIONAR ░░░░░░░░░
                                   
    ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
                  ADICIONAR PROFESSOR     [1]
                  ADICIONAR DISCIPLINA    [2]   
                  ADICIONAR ALUNO         [3]   
                        SAIR              [0]   
    
      """)
    return(chamada())
     # entrada do menu de criação    
opcao = menu_add()
while True:
    
    if opcao == 0:
        break
    
    elif opcao == 1: #criação de professor

        if add_professor() == "S":

            Professor.criar_professor()
 
    elif opcao == 2:

        if add_disciplina() == "S":
            
            Disciplina.criar_disciplina()

    elif opcao == 3:
        
        if add_aluno() == "S":
            
            Aluno.criar_aluno



