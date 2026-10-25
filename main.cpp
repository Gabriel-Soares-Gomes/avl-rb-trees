#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>
#include "include/AVLTree.h"
#include "include/LLRBTree.h"



int read_int(const std::string& prompt) {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) return val;
        std::cout << "\nEntrada invalida! Por favor, digite um numero inteiro valido.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

std::string center(const std::string& text, int width) {
    int total = std::max(0, width - static_cast<int>(text.size()));
    int left = total / 2;
    return std::string(left, ' ') + text + std::string(total - left, ' ');
}

void print_header(const std::string& title) {
    std::cout << "\n" << std::string(50, '=') << "\n"
              << center(title, 50) << "\n"
              << std::string(50, '=') << "\n";
}

void print_divider() {
    std::cout << std::string(50, '-') << "\n";
}

// ======================================================================================
//  INTERACAO COM AS ARVORES
// ======================================================================================

template <typename Tree, typename Describe>
void interact_tree(Tree& tree, const std::string& menu_title,
                   const std::string& structure_title, Describe describe) {
    while (true) {
        print_header(menu_title);
        std::cout << "1) Inserir elemento\n"
                  << "2) Remover elemento\n"
                  << "3) Buscar elemento\n"
                  << "4) Imprimir percurso em ordem\n"
                  << "5) Imprimir estrutura da arvore\n"
                  << "6) Voltar para o menu principal\n";
        print_divider();
        int op = read_int("Escolha uma opcao: ");

        if (op == 1) {
            int val = read_int("Digite o valor (ID) a ser inserido: ");
            try {
                tree.insert(val);
                std::cout << "\n[Sucesso] Elemento " << val << " inserido com sucesso!\n";
            } catch (const std::exception& e) {
                std::cout << "\n[Aviso] " << e.what() << "\n";
            }
        } else if (op == 2) {
            int val = read_int("Digite o valor (ID) a ser removido: ");
            if (tree.search(val) == nullptr) {
                std::cout << "\n[Aviso] Elemento " << val << " nao encontrado na arvore.\n";
            } else {
                tree.remove(val);
                std::cout << "\n[Sucesso] Elemento " << val << " removido com sucesso!\n";
            }
        } else if (op == 3) {
            int val = read_int("Digite o valor (ID) a ser buscado: ");
            auto node = tree.search(val);
            if (node != nullptr) {
                std::cout << "\n[Sucesso] Elemento " << val << " ENCONTRADO na arvore! ("
                          << describe(node) << ")\n";
            } else {
                std::cout << "\n[Aviso] Elemento " << val << " NAO encontrado na arvore.\n";
            }
        } else if (op == 4) {
            std::cout << "\nPercurso em ordem (in-order): ";
            tree.inorder();
        } else if (op == 5) {
            std::cout << "\n" << structure_title << ":\n";
            tree.print_tree();
        } else if (op == 6) {
            break;
        } else {
            std::cout << "\n[Erro] Opcao invalida. Tente novamente.\n";
        }
    }
}

void interact_menu(AVLTree& avl, LLRBTree& llrb) {
    while (true) {
        print_header("INTERACAO COM ARVORES");
        std::cout << "1) Interagir com a AVL\n"
                  << "2) Interagir com a RB (LLRB)\n"
                  << "3) Voltar ao menu principal\n";
        print_divider();
        int op = read_int("Escolha uma opcao: ");

        if (op == 1) {
            interact_tree(avl, "MENU - ARVORE AVL", "Estrutura da Arvore AVL",
                [](AVLNode* n) { return "Altura do no: " + std::to_string(n->get_height()); });
        } else if (op == 2) {
            interact_tree(llrb, "MENU - ARVORE RUBRO-NEGRA (LLRB)", "Estrutura da Arvore Rubro-Negra (LLRB)",
                [](LLRBNode* n) {
                    return std::string("Cor: ") + (n->get_color() == RED ? "Vermelho (R)" : "Preto (B)");
                });
        } else if (op == 3) {
            break;
        } else {
            std::cout << "\n[Erro] Opcao invalida. Tente novamente.\n";
        }
    }
}

// ======================================================================================
//  TESTES DE ESTRESSE (execucao silenciosa: so a tabela consolidada e exibida)
// ======================================================================================

// Metricas de uma arvore em um cenario. Tempo < 0 e valores <= 0 em altura,
// rotacoes e balanceamentos sao exibidos como "-" na tabela.
struct Metrics {
    double time_us = -1;
    int height = 0;
    int rotations = 0;
    int balances = 0;
};

// Uma linha da tabela: um cenario para um dado N, com as duas arvores lado a lado
struct Row {
    int n;
    std::string scenario;
    Metrics avl;
    Metrics llrb;
};

template <typename F>
double measure_us(F&& f) {
    auto t0 = std::chrono::high_resolution_clock::now();
    f();
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::micro>(t1 - t0).count();
}

template <typename Tree>
Metrics snapshot(const Tree& t, double time_us) {
    return { time_us, t.height(), t.get_rotations(), t.get_balance_count() };
}

template <typename Tree>
void fill(Tree& t, const std::vector<int>& data) {
    for (int v : data) t.insert(v);
}

template <typename Tree>
Metrics bench_insert(const std::vector<int>& data) {
    Tree t;
    double us = measure_us([&] { fill(t, data); });
    return snapshot(t, us);
}

template <typename Tree>
Metrics bench_search_existing(const std::vector<int>& data) {
    Tree t;
    fill(t, data);
    int found = 0;
    double us = measure_us([&] { for (int v : data) if (t.search(v) != nullptr) ++found; });
    volatile int sink = found; (void)sink;  // evita que a busca seja otimizada e removida
    return { us, t.height(), 0, 0 };
}

template <typename Tree>
Metrics bench_search_absent(const std::vector<int>& data) {
    Tree t;
    fill(t, data);
    const int n = static_cast<int>(data.size());
    int absent = 0;
    double us = measure_us([&] { for (int i = n + 1; i <= 2 * n; ++i) if (t.search(i) == nullptr) ++absent; });
    volatile int sink = absent; (void)sink;  // evita que a busca seja otimizada e removida
    return { us, t.height(), 0, 0 };
}

template <typename Tree>
Metrics bench_mass_removal(const std::vector<int>& data, int count) {
    Tree t;
    fill(t, data);
    t.reset_stats();
    double us = measure_us([&] { for (int i = 0; i < count; ++i) t.remove(data[i]); });
    return snapshot(t, us);
}

// Executa todos os cenarios para um N e acrescenta as linhas em `rows`
void run_stress_test_for_n(int N, std::vector<Row>& rows) {
    std::vector<int> asc(N), desc(N), rnd(N);
    for (int i = 0; i < N; ++i) {
        asc[i]  = i + 1;
        desc[i] = N - i;
        rnd[i]  = i + 1;
    }
    std::mt19937 g(42);  // seed fixa: as duas arvores recebem a mesma permutacao
    std::shuffle(rnd.begin(), rnd.end(), g);

    rows.push_back({ N, "Insercao crescente",   bench_insert<AVLTree>(asc),  bench_insert<LLRBTree>(asc)  });
    rows.push_back({ N, "Insercao decrescente", bench_insert<AVLTree>(desc), bench_insert<LLRBTree>(desc) });
    rows.push_back({ N, "Insercao aleatoria",   bench_insert<AVLTree>(rnd),  bench_insert<LLRBTree>(rnd)  });
    rows.push_back({ N, "Busca (existentes)",   bench_search_existing<AVLTree>(rnd), bench_search_existing<LLRBTree>(rnd) });
    rows.push_back({ N, "Busca (inexistentes)", bench_search_absent<AVLTree>(rnd),   bench_search_absent<LLRBTree>(rnd)   });
    rows.push_back({ N, "Remocao em massa (N/2)",
                     bench_mass_removal<AVLTree>(rnd, N / 2), bench_mass_removal<LLRBTree>(rnd, N / 2) });
}

// ======================================================================================
//  TABELA CONSOLIDADA
// ======================================================================================

namespace tbl {
    // Largura util de cada coluna (sem contar o espaco de cada lado)
    const int W_N = 6, W_SCENARIO = 24, W_TIME = 10, W_H = 5, W_ROT = 6, W_BAL = 6;
    const int W_GROUP = (W_TIME + 2) + (W_H + 2) + (W_ROT + 2) + (W_BAL + 2) + 3;

    std::string dashes(int w) { return std::string(w + 2, '-'); }

    // Linha separadora com todas as colunas
    void rule_full() {
        std::string tree_cols = dashes(W_TIME) + "+" + dashes(W_H) + "+" + dashes(W_ROT) + "+" + dashes(W_BAL);
        std::cout << "+" << dashes(W_N) << "+" << dashes(W_SCENARIO)
                  << "+" << tree_cols << "+" << tree_cols << "+\n";
    }

    // Linha separadora em que as colunas de cada arvore aparecem fundidas
    void rule_grouped() {
        std::cout << "+" << dashes(W_N) << "+" << dashes(W_SCENARIO)
                  << "+" << std::string(W_GROUP, '-') << "+" << std::string(W_GROUP, '-') << "+\n";
    }

    std::size_t utf8_length(const std::string& s) {
        std::size_t length = 0;
        for (unsigned char byte : s) {
            if ((byte & 0xC0) != 0x80) ++length;
        }
        return length;
    }

    std::string cell_left(const std::string& s, int w) {
        const std::size_t text_width = utf8_length(s);
        const std::size_t padding = text_width < static_cast<std::size_t>(w)
            ? static_cast<std::size_t>(w) - text_width
            : 0;
        return " " + s + std::string(padding, ' ') + " ";
    }
    std::string cell_right(const std::string& s, int w) {
        std::ostringstream o; o << " " << std::right << std::setw(w) << s << " "; return o.str();
    }
    std::string cell_center(const std::string& s, int w) { return " " + center(s, w) + " "; }

    std::string fmt_time(double us) {
        if (us < 0) return "-";
        std::ostringstream o;
        o << std::fixed;
        if (us >= 1000.0) o << std::setprecision(2) << us / 1000.0 << " ms";
        else              o << std::setprecision(0) << us << " us";
        return o.str();
    }

    std::string fmt_count(int v) { return v > 0 ? std::to_string(v) : "-"; }

    std::string tree_cells(const Metrics& m) {
        return cell_right(fmt_time(m.time_us), W_TIME) + "|" +
               cell_right(fmt_count(m.height), W_H) + "|" +
               cell_right(fmt_count(m.rotations), W_ROT) + "|" +
               cell_right(fmt_count(m.balances), W_BAL);
    }
}

void print_consolidated_scaling_table(const std::vector<Row>& rows) {
    using namespace tbl;
    const int total_width = 1 + (W_N + 2) + 1 + (W_SCENARIO + 2) + 1 + W_GROUP + 1 + W_GROUP + 1;

    std::cout << "\n" << center("TABELA CONSOLIDADA DE ESCALABILIDADE: AVL vs LLRB", total_width) << "\n\n";

    rule_grouped();
    std::cout << "|" << cell_center("", W_N) << "|" << cell_center("", W_SCENARIO)
              << "|" << center("AVL", W_GROUP) << "|" << center("LLRB", W_GROUP) << "|\n";
    std::cout << "|" << cell_center("N", W_N) << "|" << cell_left("Cenario", W_SCENARIO);
    for (int i = 0; i < 2; ++i) {
        std::cout << "|" << cell_center("Tempo", W_TIME) << "|" << cell_center("Alt.", W_H)
                  << "|" << cell_center("Rot.", W_ROT) << "|" << cell_center("Bal.", W_BAL);
    }
    std::cout << "|\n";
    rule_full();

    for (size_t i = 0; i < rows.size(); ++i) {
        const Row& r = rows[i];
        std::cout << "|" << cell_right(std::to_string(r.n), W_N)
                  << "|" << cell_left(r.scenario, W_SCENARIO)
                  << "|" << tree_cells(r.avl)
                  << "|" << tree_cells(r.llrb) << "|\n";

        // Linha separadora ao final de cada bloco de N
        bool last_of_block = (i + 1 == rows.size()) || (rows[i + 1].n != r.n);
        if (last_of_block) rule_full();
    }

    std::cout << "\nLegenda: Alt. = altura final | Rot. = rotacoes | Bal. = balanceamentos\n"
              << "         Tempo em us (microssegundos) ou ms (milissegundos) | '-' = nao se aplica\n";
}

// ======================================================================================
//  CASOS DE BORDA (secao separada, exibida abaixo da tabela consolidada)
// ======================================================================================

struct EdgeCase {
    std::string name;
    bool avl_ok;
    bool llrb_ok;
};

template <typename Tree>
bool check_empty_tree() {
    Tree t;
    if (t.height() != 0 || t.count_nodes() != 0 || t.search(10) != nullptr) return false;
    t.remove(10);  // nao deve travar nem alterar a arvore
    return t.count_nodes() == 0;
}

template <typename Tree>
bool check_single_element() {
    Tree t;
    t.insert(42);
    return t.height() == 1 && t.count_nodes() == 1 && t.search(42) != nullptr;
}

template <typename Tree>
bool check_remove_single_element() {
    Tree t;
    t.insert(42);
    t.remove(42);
    return t.height() == 0 && t.count_nodes() == 0 && t.search(42) == nullptr;
}

// Remove `id` de uma arvore base e confere: chave ausente e quantidade de nos
template <typename Tree>
bool check_removal(int id) {
    const std::vector<int> base = {50, 25, 75, 15, 35, 65, 85, 10, 30, 40};
    Tree t;
    fill(t, base);
    bool existed = (t.search(id) != nullptr);
    t.remove(id);
    int expected = static_cast<int>(base.size()) - (existed ? 1 : 0);
    return t.search(id) == nullptr && t.count_nodes() == expected;
}

std::vector<EdgeCase> run_edge_cases() {
    return {
        { "Árvore vazia (busca e remoção)", check_empty_tree<AVLTree>(),            check_empty_tree<LLRBTree>() },
        { "Árvore com 1 elemento",          check_single_element<AVLTree>(),        check_single_element<LLRBTree>() },
        { "Remoção do único elemento",      check_remove_single_element<AVLTree>(), check_remove_single_element<LLRBTree>() },
        { "Remoção de nó folha (10)",       check_removal<AVLTree>(10),             check_removal<LLRBTree>(10) },
        { "Remoção de nó com 1 filho (15)", check_removal<AVLTree>(15),             check_removal<LLRBTree>(15) },
        { "Remoção de nó com 2 filhos (25)",check_removal<AVLTree>(25),             check_removal<LLRBTree>(25) },
        { "Remoção da raiz (50)",           check_removal<AVLTree>(50),             check_removal<LLRBTree>(50) },
        { "Remoção de chave inexistente",   check_removal<AVLTree>(9999),           check_removal<LLRBTree>(9999) },
    };
}

void print_edge_cases_table(const std::vector<EdgeCase>& cases) {
    using namespace tbl;
    const int W_CASE = 36, W_RES = 7;
    const int total_width = 1 + (W_CASE + 2) + 1 + (W_RES + 2) + 1 + (W_RES + 2) + 1;
    const std::string rule = "+" + dashes(W_CASE) + "+" + dashes(W_RES) + "+" + dashes(W_RES) + "+\n";

    std::cout << "\n\n" << center("CASOS DE BORDA E REMOCOES ESTRUTURAIS", total_width) << "\n\n";
    std::cout << rule
              << "|" << cell_left("Caso", W_CASE) << "|" << cell_center("AVL", W_RES) << "|" << cell_center("LLRB", W_RES) << "|\n"
              << rule;

    int failures = 0;
    for (const auto& c : cases) {
        failures += (!c.avl_ok) + (!c.llrb_ok);
        std::cout << "|" << cell_left(c.name, W_CASE)
                  << "|" << cell_center(c.avl_ok ? "OK" : "FALHA", W_RES)
                  << "|" << cell_center(c.llrb_ok ? "OK" : "FALHA", W_RES) << "|\n";
    }
    std::cout << rule;

    if (failures == 0) std::cout << "\nTodos os casos de borda passaram nas duas arvores.\n";
    else               std::cout << "\nAtencao: " << failures << " verificacao(oes) com falha.\n";
}


void run_and_print(const std::vector<int>& sizes) {
    std::vector<Row> rows;
    for (int n : sizes) run_stress_test_for_n(n, rows);

    print_consolidated_scaling_table(rows);
    print_edge_cases_table(run_edge_cases());
}

void automatic_tests_menu() {
    while (true) {
        print_header("MENU - TESTES AUTOMATICOS E ESTRESSE");
        std::cout << "1) Executar bateria completa (1000, 2000, 4000 e 8000 elementos)\n"
                  << "2) Definir quantidade personalizada de elementos\n"
                  << "3) Voltar ao menu principal\n";
        print_divider();
        int op = read_int("Escolha uma opcao: ");

        if (op == 1) {
            run_and_print({1000, 2000, 4000, 8000});
        } else if (op == 2) {
            std::cout << "\n[Info] A quantidade deve estar entre 10 e 50.000 elementos.\n";
            int n = read_int("Digite a quantidade de elementos desejada (10 a 50000): ");
            if (n < 10) {
                std::cout << "\n[Erro] Quantidade muito pequena! O minimo e 10 elementos.\n";
            } else if (n > 50000) {
                std::cout << "\n[Erro] Quantidade excessiva! O maximo seguro e 50.000 elementos.\n";
            } else {
                run_and_print({n});
            }
        } else if (op == 3) {
            break;
        } else {
            std::cout << "\n[Erro] Opcao invalida! Escolha 1, 2 ou 3.\n";
        }
    }
}


int main() {
    AVLTree avl_tree;
    LLRBTree llrb_tree;

    while (true) {
        print_header("MENU PRINCIPAL");
        std::cout << "1) Interagir com as árvores\n"
                  << "2) Realizar testes automáticos\n"
                  << "3) Sair\n";
        print_divider();
        int opcao = read_int("Escolha uma opção: ");

        if (opcao == 1) {
            interact_menu(avl_tree, llrb_tree);
        } else if (opcao == 2) {
            automatic_tests_menu();
        } else if (opcao == 3) {
            std::cout << "\nEncerrando o programa. Até logo!\n";
            break;
        } else {
            std::cout << "\nOpção inválida! Escolha 1, 2 ou 3.\n";
        }
    }

    return 0;
}
