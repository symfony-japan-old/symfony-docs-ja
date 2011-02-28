CollectionField
===============

``CollectionField`` は、配列や ``Traversable`` インタフェースを実装したオブジェクトを操作するための特別なフィールドグループです。このデモを実施するために、3つのEメールアドレスを保存できるよう ``Customer`` クラスを拡張しました。

    class Customer
    {
        // other properties ...

        public $emails = array('', '', '');
    }

ここでアドレスを操作できるように ``CollectionField`` を加えます。

    use Symfony\Component\Form\CollectionField;

    $form->add(new CollectionField('emails', array(
        'prototype' => new EmailField(),
    )));

 "modifiable" オプションを ``true`` に設定する場合、JavaScript を使ってコレクションに行を追加したり削除することもできます！ ``CollectionField`` がこれを知らせてくれて、基本となる配列を適切にリサイズしてくれるでしょう。
