// struct product createProduct(){
//     struct product product;
//     printf("商品番号を入力してください：");
//     scanf("%d", &product.no);
//     printf("名簿コードを入力してください：");
//     scanf("%s", product.code);
//     printf("商品名を入力してください：");
//     scanf("%s", product.name);
//     printf("単価を入力してください：");
//     scanf("%d", &product.price);
//     printf("商品の数を入力してください：");
//     scanf("%d", &product.inventory);
//     return product;
// }
// struct change createChange(){
//     struct change change;
//     printf("Noを入力してください：");
//     scanf("%d", &change.no);
//     printf("金銭の金額を入力してください：");
//     scanf("%d", &change.kingaku);
//     printf("枚数を入力してください：");
//     scanf("%d", &change.quan);
//     // change.amount = change.kingaku * change.quan;
//     return change;
// }
// struct Transaction createTransaction() {
//     struct Transaction Transaction;
//     // struct product product;
//     printf("レシート番号を入力してください：");
//     scanf("%d", &Transaction.TransactionNo);
//     printf("日付を入力してください：");
//     scanf("%s", &Transaction.date);
//     printf("名簿コードを入力してください：");
//     scanf("%s", &Transaction.product_r.code);
//     printf("商品名を入力してください：");
//     scanf("%s", &Transaction.product_r.name);
//     printf("販売数量を入力してください：");
//     scanf("%d", &Transaction.product_r.sold_quantity);
//     printf("商品単価を入力してください：");
//     scanf("%d", &Transaction.product_r.price);
//     printf("売上高を入力してください：");
//     scanf("%d", &Transaction.amount);
//     return Transaction;
// }