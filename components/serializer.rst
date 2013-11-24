.. index::
   single: Serializer
   single: Components; Serializer

.. note::

    * 対象バージョン : 2.3
    * 翻訳更新日 : 2013/11/25

Serializerコンポーネント
========================

   Serializer コンポーネントは、オブジェクトを別のフォーマット（XML、JSON、 Yamlなど）に変更するのに使われます。

そのために、 Serializer コンポーネントは下図のようなシンプルな仕組みに従っています。

.. _component-serializer-encoders:
.. _component-serializer-normalizers:

.. image:: /images/components/serializer/serializer_workflow.png

上の図を見るとわかるように、配列（Array）が仲介として使われています。
つまり、 Encoder は **フォーマット** を **配列** に変換したり元に戻したりするのを担当し、 Normalizer は **オブジェクト** を **配列** に変換したり戻したりするのを担当しています。

シリアライズは複雑な課題であり、Serializer コンポーネントはすべてのケースに使えるわけではありませんが、開発者がシリアライズしたいオブジェクトをシリアライズする道具を作るのに役立つでしょう。

インストール
---------------

コンポーネントをインストールする方法は2つあります。

* :doc:`Composer </components/using_components>` (`Packagist`_ の ``symfony/serializer``)
* 公式Gitレポジトリ (https://github.com/symfony/Serializer)

使い方
--------

Serializer コンポーネントの使い方は、本当にシンプルです。利用できる Encoder と Normalizer を指定して :class:`Symfony\\Component\\Serializer\\Serializer` を開始するだけです。::

    use Symfony\Component\Serializer\Serializer;
    use Symfony\Component\Serializer\Encoder\XmlEncoder;
    use Symfony\Component\Serializer\Encoder\JsonEncoder;
    use Symfony\Component\Serializer\Normalizer\GetSetMethodNormalizer;

    $encoders = array(new XmlEncoder(), new JsonEncoder());
    $normalizers = array(new GetSetMethodNormalizer());

    $serializer = new Serializer($normalizers, $encoders);

オブジェクトをシリアライズする
-------------------------------

例を動かしてみる前に、下のクラスがプロジェクトに含まれていることを確認してください。::

    namespace Acme;

    class Person
    {
        private $age;
        private $name;

        // Getters
        public function getName()
        {
            return $this->name;
        }

        public function getAge()
        {
            return $this->age;
        }

        // Setters
        public function setName($name)
        {
            $this->name = $name;
        }

        public function setAge($age)
        {
            $this->age = $age;
        }
    }

この Person オブジェクトを JSON にシリアライズしたいなら、先に作った Serializer サービスをこのように使うだけです。::

    $person = new Acme\Person();
    $person->setName('foo');
    $person->setAge(99);

    $jsonContent = $serializer->serialize($person, 'json');

    // $jsonContent には {"name":"foo","age":99} が入ります

    echo $jsonContent; // もしくは、Responseに渡すこともできます

:method:`Symfony\\Component\\Serializer\\Serializer::serialize` メソッドの第一引数はシリアライズしたいオブジェクト、第二引数は使いたい encoder を指定するのに使います。
encoder は、この例では :class:`Symfony\\Component\\Serializer\\Encoder\\JsonEncoder` です。

シリアライズ時に属性を無視する
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    :method:`GetSetMethodNormalizer::setIgnoredAttributes<Symfony\\Component\\Serializer\\Normalizer\\GetSetMethodNormalizer::setIgnoredAttributes>` メソッドは Symfony 2.3 で追加されました。

オプションとして、シリアライズ時に元のオブジェクトの属性を無視させることもできます。
属性を無視させたいときは、 normalizer の定義時に :method:`Symfony\\Component\\Serializer\\Normalizer\\GetSetMethodNormalizer::setIgnoredAttributes`
を使います。::

    use Symfony\Component\Serializer\Serializer;
    use Symfony\Component\Serializer\Encoder\JsonEncoder;
    use Symfony\Component\Serializer\Normalizer\GetSetMethodNormalizer;

    $normalizer = new GetSetMethodNormalizer();
    $normalizer->setIgnoredAttributes(array('age'));
    $encoder = new JsonEncoder();

    $serializer = new Serializer(array($normalizer), array($encoder));
    $serializer->serialize($person, 'json'); // シリアライズ結果は {"name":"foo"} になります

オブジェクトをデシリアライズする
---------------------------------

今度はちょうど反対の処理を見てみましょう。
``Person`` クラスの情報は、 XML にエンコードされています。::

    $data = <<<EOF
    <person>
        <name>foo</name>
        <age>99</age>
    </person>
    EOF;

    $person = $serializer->deserialize($data,'Acme\Person','xml');

この例では、 :method:`Symfony\\Component\\Serializer\\Serializer::deserialize` メソッドには3つの引数が必要です。

1. デコードする情報
2. デコード先のクラスの名前
3. 情報を配列に変換するのに使う encoder

アンダースコア付きの属性にキャメルケース式のメソッド名を使う
-------------------------------------------------------------

.. versionadded:: 2.3
    :method:`GetSetMethodNormalizer::setCamelizedAttributes<Symfony\\Component\\Serializer\\Normalizer\\GetSetMethodNormalizer::setCamelizedAttributes>`　は Symfony 2.3 で追加されました。

シリアライズ後の形式に含まれる属性名にアンダースコアがついていることがあります。（たとえば、 ``first_name`` のように）
もし ``getFirstName`` のようなメソッド名を使いたいとしても、通常はアンダースコア付きの属性には ``getFirst_name`` のようなメソッド名でゲッター・セッターが使われてしまいます。
そのような場合には、 normalizer を定義する時に :method:`Symfony\\Component\\Serializer\\Normalizer\\GetSetMethodNormalizer::setCamelizedAttributes` メソッドを使います。::

    $encoder = new JsonEncoder();
    $normalizer = new GetSetMethodNormalizer();
    $normalizer->setCamelizedAttributes(array('first_name'));

    $serializer = new Serializer(array($normalizer), array($encoder));

    $json = <<<EOT
    {
        "name":       "foo",
        "age":        "19",
        "first_name": "bar"
    }
    EOT;

    $person = $serializer->deserialize($json, 'Acme\Person', 'json');

こうすると、デシリアライズ時に ``first_name`` は ``firstName`` であるかのように扱われ、メソッド名として ``getFirstName`` と ``setFirstName``  が使われるようになります。

JMSSerializer
-------------

サードパーティのライブラリ `JMS serializer`_ は、もっと複雑な方式ではありますが、より洗練されています。
このライブラリは、オブジェクトをどのようにシリアライズ・デシリアライズするべきかをアノテーション（YML,XML,PHPでも同様）により設定することができ、 Doctrine ORM や循環参照があるオブジェクトでも扱うことができます。

.. _`JMS serializer`: https://github.com/schmittjoh/serializer
.. _Packagist: https://packagist.org/packages/symfony/serializer

.. 2013/11/25 77web 850fde93481c208fe74d5cfa898302ec708238a7
