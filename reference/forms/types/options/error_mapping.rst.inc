error_mapping
~~~~~~~~~~~~~

**データ型**: ``array`` **デフォルト**: ``empty``

このオプションはバリデーションエラーの対象を編集することができます。

``matchingCityAndZipCode`` という名前のメソッドを持っているとして、それが ``city`` および ``zip code`` が一致するかの検証をするとします。しかし残念なことに、 "matchingCityAndZipCode" フィールドがフォームに無いため、 Symfony ができるのはフォームの上部にエラーを表示することです。

カスタマイズされたエラーマッピングを使うと、``city`` フィールドの上にエラーを表示するように、より上手にエラーをマップできます。::

    public function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        $resolver->setDefaults(array(
            'error_mapping' => array(
                'matchingCityAndZipCode' => 'city',
            ),
        ));
    }

こちらが左側、右側のマッピングするルールです:

* 左側はプロパティパスを有します。

* クラスのプロパティまたはメソッドで違反が生成された場合、そのパスは単に "propertyName" です。

* ``array`` または ``ArrayAccess`` オブジェクトで違反が生成された場合、プロパティパスは ``[indexName]`` です。

* ドットでプロパティを分離し、それらを連結して、ネストされたプロパティパスを作成することができます。たとえば ``addresses[work].matchingCityAndZipCode`` のように。

* 左側のエラーマッピングは単一のドット ``.`` も認められます。それはフィールド自体をさしています。フィールドに追加されたエラーはネストされたフィールドの代わりに付与されたことを意味します。

* 右側は単にフォームのフィールド名を有します。

.. 2013/12/04 yositani2020 343a59e9ea58ed750d1c6211db349d3b2cc4352e
