.. note::

    * 対象バージョン：2.3 (2.1以降)
    * 翻訳更新日：2013/11/23

attr
~~~~

**type**: array **default**: Empty array

HTMLのボタンに特別な属性を追加したい場合は、``attr`` オプションを使うことができます。
これは、HTML属性の連想配列のキーとなります。ボタンにカスタムクラスを設定する時に便利です。::

    $builder->add('save', 'button', array(
        'attr' => array('class' => 'save'),
    ));


.. 2013.11.23 kang1123 6c883642848c9aaf2a425774b8ddf13737159248